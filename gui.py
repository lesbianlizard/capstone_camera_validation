import zmq

context = zmq.Context()

#  Socket to talk to server
print("----------- WELCOME TO THE CORRUPTION GUI ------------")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

# send commands over the socket
while True: 
    cmd = input("CMD: ")
    print("Sending request {}".format(cmd))
    socket.send_string(cmd)

    #  Get the reply.
    message = socket.recv()
    print(message.decode('utf-8'))
    if message.decode('utf-8') != "ACK":
        print("Did not receive ACK, exiting GUI")
        exit()


# Format (use .join(',') method on a dynamically constructed list object) to produce these commands
# Freeze:   ['1', 't/f', 'dur(ms)']
# White:    ['2', 't/f', 'dur(ms)', 'shade(0-255)']
# Translate ['3', 't/f', 'dur(ms)', 'x_offset', 'y_offset']