import zmq

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server...")
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


# Format (use .join(',') method for csv of commands
# Freeze:   ['1', 'dur(ms)', 't/f']
# Shade:    ['2', 'dur(ms)', 't/f', 'shade(0-255)']
# Translate ['3', 'dur(ms)', 't/f', 'x_offset', 'y_offset']