from __future__ import annotations
import asyncio
import socket
import struct
from typing import List, Optional
from Protobuf.Output import DNDMapper_pb2
import datetime


class Game:
    def __init__(self, game_id: int):
        self.DM: Optional[Player] = None
        self.players: List[Player] = []
        self.message_history = []
        self.game_id = game_id

    async def send_update_message(self, sender: Player, message):
        if message.HasField("map"):
            self.message_history.clear()
        self.message_history.append(message)

        if sender is not self.DM:
            self.DM.message_queue.append(message)
            # print("Sent message to DM in game:", self.game_id)
        for player in self.players:
            if player is not sender:
                # print("Sent message to player in game", self.game_id)
                player.message_queue.append(message)

    async def add_player(self, player: Player):
        if player not in self.players:
            self.players.append(player)
            player.message_queue.clear()
            player.message_queue.extend(self.message_history)  # Add the history of the game since last map update to catch player up to speed.

    async def is_DM(self, player: Player):
        return self.DM == player

    async def is_player(self, player: Player):
        return player in self.players

    async def remove_player(self, player: Player):
        if player is self.DM:  # If we remove DM, game's done for.
            self.DM = None
            self.players.clear()
        elif player in self.players:
            self.players.remove(player)
        player.message_queue.clear()


class Player:
    def __init__(self, writer: asyncio.streams.StreamWriter):
        self.writer = writer
        self.sock: socket.socket = self.writer.get_extra_info('socket')
        self.message_queue = []
        print("Added player", self.sock.getpeername())


async def send_message(writer: asyncio.streams.StreamWriter, message):
    # print("Sent message")
    s = message.SerializeToString()
    len_bytes = struct.pack('<L', len(s))
    writer.write(len_bytes + s)
    await writer.drain()


class Server:

    def __init__(self):
        self.players: List[Player] = []
        self.games: List[Game] = []
        self.second_message_count = 0
        self.second_message_timer = datetime.datetime.now()

    def run_server(self):
        asyncio.get_event_loop().create_task(asyncio.start_server(self.handle_client, '0.0.0.0', 51248))
        asyncio.get_event_loop().create_task(self.catchup_players())
        asyncio.get_event_loop().run_forever()

    async def catchup_players(self):
        while True:
            for game in self.games:
                for player in game.players + [game.DM]:
                    while len(player.message_queue) > 0:
                        await send_message(player.writer, player.message_queue[0])
                        player.message_queue.remove(player.message_queue[0])

            # Message analytics.
            if (datetime.datetime.now() - self.second_message_timer) >= datetime.timedelta(seconds=1):
                print("Messages/second: ", self.second_message_count)
                self.second_message_timer = datetime.datetime.now()
                self.second_message_count = 0

            # Sleep
            await asyncio.sleep(0.01)

    async def handle_client(self, reader: asyncio.streams.StreamReader, writer: asyncio.streams.StreamWriter):
        if await self.get_player(writer) is None:
            await self.add_player(writer)

            while True:
                try:

                    # Read first 4 bytes to get length.
                    msg_len_bytes = b''
                    while len(msg_len_bytes) < 4:
                        msg_len_bytes += await reader.read(4)

                    if not msg_len_bytes:
                        print("Client Disconnected")
                        await self.cleanse_player(writer)
                        break

                    msg_len = struct.unpack('<L', msg_len_bytes)[0]
                    data = b''
                    while len(data) < msg_len:
                        data += await reader.read(msg_len - len(data))

                    # print(data)

                    # message analytics.
                    self.second_message_count += 1

                    # Here's where we really do stuff.

                    message = DNDMapper_pb2.NetworkMessage()
                    message.ParseFromString(data)
                    # print(message)
                    if message.messageType == 1:  # CreateGame
                        new_game = Game(await self.get_fresh_game_id())
                        new_game.DM = await self.get_player(writer)
                        self.games.append(new_game)

                        # Send back the ID for the client.
                        id_message = DNDMapper_pb2.NetworkMessage()
                        id_message.messageType = 4
                        id_message.gameID = new_game.game_id
                        await send_message(writer, id_message)

                    elif message.messageType == 2:  # JoinGame
                        if message.HasField("gameID"):
                            game_id = message.gameID
                            game: Game = await self.get_game_from_id(game_id)
                            if game:
                                await game.add_player(await self.get_player(writer))
                                # Send back the ID for the client.
                                id_message = DNDMapper_pb2.NetworkMessage()
                                id_message.messageType = 4
                                id_message.gameID = game_id
                                await send_message(writer, id_message)
                            else:  # We send an error message back.
                                error_message = DNDMapper_pb2.NetworkMessage
                                error_message.messageType = 5
                                error_message.error = 1
                                await send_message(writer, error_message)
                    else:  # Update Message
                        sender = await self.get_player(writer)
                        game = await self.get_game_from_player(sender)
                        await game.send_update_message(sender, message)

                except ConnectionResetError:
                    print("Client Disconnected")
                    await self.remove_player(writer)
                    break

        else:
            await self.remove_player(writer)
            writer.close()
            await writer.wait_closed()

    async def get_game_from_id(self, game_id: int):
        for game in self.games:
            if game.game_id == game_id:
                return game
        return None

    async def get_game_from_player(self, player: Player):
        for game in self.games:
            if player is game.DM or player in game.players:
                return game
        return None

    async def get_fresh_game_id(self):
        new_id = 0
        should_continue = True
        while should_continue:
            should_continue = False
            for game in self.games:
                if game.game_id == new_id:
                    new_id += 1
                    should_continue = True

        return new_id


    async def cleanse_player(self, writer: asyncio.streams.StreamWriter):
        player = await self.get_player(writer)
        for game in self.games:
            await game.remove_player(player)
            await self.remove_player(writer)
            if game.DM is None:
                self.games.remove(game)

    async def add_player(self, writer: asyncio.streams.StreamWriter):
        self.players.append(Player(writer))

    async def get_player(self, writer: asyncio.streams.StreamWriter):
        for player in self.players:
            if player.writer == writer:
                return player

        return None

    async def remove_player(self, writer: asyncio.streams.StreamWriter):
        for player in reversed(self.players):
            if player.writer == writer:
                self.players.remove(player)

server = Server()
server.run_server()