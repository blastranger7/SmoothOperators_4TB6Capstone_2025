from flask import Flask, request, jsonify
from filelock import FileLock, Timeout
from ctypes import c_uint8
from ctypes import c_uint16
import serial
import json
import struct
import threading

app = Flask(__name__)

# Probably unnecessary now
API_KEYS = {
    "object_detection_system": "ods_key",
    "robot_software": "rs_key"
}

# Shared resource to store incoming data
serial_data = []
angle = []

# Initialize UART
ser0 = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1)
ser1 = serial.Serial('/dev/ttyAMA2', baudrate=115200, timeout=1)

def read_serial0_data():
    """Function to read serial data continuously"""
    global serial_data
    while True:
        data = ser0.read(1)  # Assume that MCU will only send data via serial 0

        if len(data) == 1:
            unpacked_data = struct.unpack('BBB', data)
            string_data = list(map(str, unpacked_data))
            print(f"Received from UART: {string_data}")
            serial_data.append(string_data)
            
def read_serial1_data():
    """Function to read serial data continuously"""
    global angle
    while True:
        data = ser1.read(1)  # Assume that MCU will only send data via serial 0

        if len(data) == 1:
            unpacked_data = struct.unpack('BBB', data)
            string_data = list(map(str, unpacked_data))
            print(f"Received from UART: {string_data}")
            angle.append(string_data)

# Define the endpoint to receive data
@app.route('/write', methods=['POST'])
def write_to_file():
    try:

        if ser0.is_open and ser1.is_open:
            print("Serial connection established successfully.")

        else:
            if not ser0.is_open:
                print("Failed to open serial port 0.")
            if not ser1.is_open:
                print("Failed to open serial port 1.")

        # Get JSON data from the request
        data = request.get_json()   # Python dictionary
        if not isinstance(data, dict):
            return {"error": "Expected a dictionary"}, 400

        commands = data.get('message')
        if not isinstance(commands, list):
            return {"error": "Expected a list for 'message'"}, 400

        # Actual code starts here
        lock = FileLock('updated_can_state.txt.lock', timeout = 5)

        formatted_commands = [c_uint16(element) for element in commands]

        # Write data to a text file to track most recent changes
        with open('received_data.txt', 'w') as file:
            for command in commands:
                file.write(str(command) + " - type " + str(type(command)) + '\n')

        # Send serial data to microcontroller (18 bytes total)
        packet = struct.pack('HHH', formatted_commands[0].value, formatted_commands[1].value, formatted_commands[2].value)
        print_packet = list(packet)
        print(print_packet)
        # Fix sized packet created to be sent to MCU
        if len(commands) == 3:
            if commands[0] == 2: # 2 = 'MoveCan', change if needed.
                try:
                    with lock:
                        with open('updated_can_state.txt', 'r') as can_file:
                            content = can_file.read()
                except Timeout:
                    return {'status': 'error', 'message': 'updated_can_state.txt file being used by different process.'}, 500
                if "1" not in content: # '1' = 'CanInWorkspace', change if needed.
                    return {'status': 'error', 'message': 'Can is not in the workspace'}, 500
            # Send serial data to microcontroller (18 bytes total)
            ser0.write(packet)
            print("ser0 wrote")
        elif len(commands) == 4:
            try:
                with lock:
                    with open('updated_can_state.txt', 'w') as can_file:
                        can_file.write(f'{commands[0]}')
            except Timeout:
                return {'status': 'error', 'message': 'updated_can_state.txt file being used by different process.'}, 500
            except Exception as e:
                return {'status': f'error {e}', 'message': 'updated_can_state.txt file being used by different process.'}, 500
            ser1.write(packet)
            print("ser1 wrote")


        # # Indicate that state of the can should be updated
        # with open('updated_can_state.txt', 'w') as can_file:
        #     can_file.write("Update" + '\n')

        # Close serial connection
        # ser0.close()
        # ser1.close()

        return {'status': 'success', 'message': 'Data written to file'}, 200
    except Exception as e:
        return {'status': 'error', 'message': str(e)}, 500

@app.route('/read_serial1', methods=['GET'])
def read_serial1():

    return {"received_data": serial_data}, 200

    # try:
    #     ser1 = serial.Serial('/dev/serial1', baudrate=115200, timeout=1)

    #     if not ser1.is_open:
    #         return {'status': 'error', 'message': 'Serial1 is not open'}, 500
    #     else:
    #         print("Serial connection established successfully.")

    #     print(ser1.in_waiting)
    #     i = 0
    #     while ser1.in_waiting == 0:
    #         i += 1
    #         if i >= 100000:
    #             break

    #     if ser1.in_waiting > 0:
    #         print("1")
    #         received_data = ser1.read(ser1.in_waiting)  # Read all available data
    #         received_data_str = received_data.decode('utf-8').strip()

    #         with open('received_serial1.txt', 'w') as file:
    #             file.write(received_data_str)

    #         return {'status': 'success', 'data': received_data_str}, 200
    #     else:
    #         print("2")
    #         return {'status': 'success', 'message': 'No data available'}, 200
    # except Exception as e:
    #     print("3")
    #     return {'status': 'error', 'message': str(e)}, 500


if __name__ == '__main__':

    serial0_thread = threading.Thread(target=read_serial0_data, daemon=True)
    serial0_thread.start()
    serial1_thread = threading.Thread(target=read_serial1_data, daemon=True)
    serial1_thread.start()

    app.run(host='0.0.0.0', port=5000)
