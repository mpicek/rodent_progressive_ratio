import serial
import datetime
import msvcrt
import serial.tools.list_ports

port = 'COM5' # default port
use_default_port = False


# how many presses is needed to obtain a reward (at each level)
presses_needed = [1, 2, 4, 8, 100000]
# how many times do we stay at the same level
level_repeats = [3, 3, 2, 1, 100000]

level = 0 # current level of difficulty to obtain reward
repeats = 0 # how many times the rodent got a reward at the current level
presses = 0 # how many presses has the rodent done without obtaining the reward

baud_rate = 9800
log_into_console = True

# these are the commands to be sent to the arduino
reward = b'p' # arduino gives a reward to the rodent
start = b's'  # signifies the start of the experiment,
# arduino returns the time of the experiment's start

def button_pressed():
    """
    A function that is run when the rodent presses the button.
    """
    
    # access the global variables
    global level
    global repeats
    global presses
    
    presses += 1
    
    # if the rodent did enough presses to obtain a
    # reward at the current level
    if presses == presses_needed[level]:
        repeats += 1
        presses = 0
        ser.write(reward) # give a reward to the rodent
        
    if repeats == level_repeats[level]:
        repeats = 0
        level += 1

def find_arduino_port():
    """
    Tries to find a port where the Arduino is connected.
    If no port is found, the default port is used. Otherwise, the newly
    found port is used.
    """
    global port # access the global variable (where the default value is stored)

    ports = list(serial.tools.list_ports.comports())
    arduino_found = False
    
    for p in ports: # scan all ports
        if "uino" in p.description.lower(): # there are different types of Arduino (Genuino)
            print("Found Arduino device: ", end='')
            print(p.description)
            print("Therefore, port " + p.device + " will be used.")
            port = p.device
            arduino_found = True
            break
        
    if not arduino_found:
        print("No Arduino device found. The default port "+ str(port) + " will be used.")

if not use_default_port:
    find_arduino_port()

ser = serial.Serial(port, baud_rate, timeout=1)
now = datetime.datetime.now()
file_name = now.strftime("%Y-%m-%d_%H-%M.csv")
print("Creating a file " + file_name)
print("End the experiment by typing \"e\"")
print()
log = open(file_name, "w") # create the logging file
# write a header into the logging file
log.write("time;event;presses_needed;level_occurance;presses\n")

ser.write(start)

while True:
    arduino_serial = ser.readline().decode().strip() # parse incoming messages from Arduino
    if arduino_serial: # if a message came from the Arduino
        log_str = ""
        
        log_str = str(arduino_serial) + ";" + str(presses_needed[level]) + ";" + str(repeats + 1) + ";" + str(presses + 1) + "\n"
        
        if "button" in str(arduino_serial): # if the event is a button release, handle the event
            button_pressed()
        
        if log_into_console:
            print(log_str, end="")
        
        log.write(log_str) # log the event
        log.flush() # save the file
    
    # check whether the user did not entered text via keyboard
    if msvcrt.kbhit():
        input_str = msvcrt.getch().decode()
        if input_str == 'e' or input_str == "E":
            break
log.close()

