import socket, threading, time

HOST = '127.0.0.1'
PORT = 3000
N = 50

def worker(i):
    try:
        s = socket.create_connection((HOST, PORT), timeout=3)
        s.sendall(f"client->{i}\n".encode())
        resp = s.recv(4096)
        lines = resp.decode(errors='ignore').splitlines()
        print(f"\n{'='*30}\nClient {i} session started at {time.strftime('%H:%M:%S')}")
        for idx, line in enumerate(lines):
            print(f"  [{idx+1}] {line}")
        print(f"Client {i} received {len(lines)} messages.")
        print(f"Client {i} session ended at {time.strftime('%H:%M:%S')}\n{'='*30}\n")
        time.sleep(1)
        s.close()
    except Exception as e:
        print(f"\n{'='*30}\nClient {i} session started at {time.strftime('%H:%M:%S')}")
        print(f"  Error: {e}")
        print(f"Client {i} session ended at {time.strftime('%H:%M:%S')}\n{'='*30}\n")

threads = []
for i in range(N):
    t = threading.Thread(target=worker, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()
