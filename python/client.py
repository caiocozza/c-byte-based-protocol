import sys
import socket

class PClient:
    def __init__(self, key: str, address: str, port: int) -> None:
        self.__key = key
        self.__address = address
        self.__port = port
        self.__services = list()

        self.__get_services()

    def connect(self) -> None:
        try:
            self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.__socket.connect((self.__address, self.__port))
        except:
            print("Could not connect to the server.", file=sys.stderr)

    def __get_services(self):
        self.connect()
        buffer = bytearray()
        self.__socket.send(b"cmd\n0\n")

        rec = self.__socket.recv(1024)
        while b"\r\r" not in buffer:
            buffer.extend(rec)
            rec = self.__socket.recv(4)

        buffer = buffer.split(b"\n")
        buffer.pop()

        self.__services = [sub.decode() for sub in buffer]
        return 0

    def send(self, service: str, data) -> None:
        if service in self.__services:
            self.connect()
            body = b"\n"+self.__key.encode()+b"\n"+str(self.__services.index(service)).encode()+b"\n"+str(data).encode()+b"\n"
            header_len = str(len(body))

            header = '0'*32
            header = header[len(header_len):]
            header+=header_len

            self.__socket.send(header.encode()+body)

            buffer = bytearray()
            rec = self.__socket.recv(4)
            while b"\r\n" not in buffer:
                buffer.extend(rec)
                rec = self.__socket.recv(4)

            buffer = buffer[:len(buffer)-2]
            return buffer.decode()

        else:
            print('not in services')

pclient = PClient("123456789", "127.0.0.1", 8888)
print(pclient.send('user/find', 'test'))