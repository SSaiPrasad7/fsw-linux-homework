'''
This script primarily serves as an introductory exploration into receiving data.
'''
import socket
import time

class SocketCommunicator:
    HOST = "127.0.0.1"  

    def __init__(self) -> None:
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def open(self, port):
        connection_status = self.sock.connect_ex((self.HOST, port))
        if connection_status != 0:
            print("Connection refused for port : ", port)
            return False
        return True
    
    def close(self):
        self.sock.close()

    def recv(self):
        data = b""
        while True:
            readbyte = self.sock.recv(1)
            if readbyte == b'':
                return None
            if readbyte == b"\n":
                break
            data += readbyte

        return data.decode('utf-8')
    
    def timeout(self, timeout=1):
        self.sock.timeout(timeout) 

if __name__ == "__main__":
    socket1 = SocketCommunicator()
    socket2 = SocketCommunicator()
    socket3 = SocketCommunicator()

    status1 = socket1.open(4001)
    status2 = socket2.open(4002)
    status3 = socket3.open(4003)

    if status1 and status2 and status3:
        while True:
            out1 = socket1.recv()
            out2 = socket2.recv()
            out3 = socket3.recv()
            epoch_time_seconds = time.time()
            epoch_time_milliseconds = int(epoch_time_seconds * 1000)
            out_string = f'{{"timestamp": {epoch_time_milliseconds}, "out1": "{out1}", "out2": "{out2}", "out3": "{out3}"}}'
            print(out_string)
    socket1.close()
    socket2.close()
    socket3.close()

