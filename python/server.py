import ctypes

lib = ctypes.cdll.LoadLibrary('./protocol.so')

SERVICE = ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_char_p)

class PTesting:
    def __init__(_) -> None:
        lib.create_manager()

    def listen(_, th: int, port: int, host: str):
        lib.setup_socket_thread_pool(port, th, host)

    def create_service(_, path: str, fn) -> int:
        local_fn_l = SERVICE(fn)
        return lib.create_service(path, local_fn_l)

    @staticmethod
    def send(client, data):
        lib.send_response(client, data.encode())


protocol = PTesting()

def users(client, data):
    print(data)
    PTesting.send(client, "testing...")

print(protocol.create_service("get/users", users))
protocol.listen(4, 8888, "127.0.0.1")