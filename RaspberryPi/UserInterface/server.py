from flask import Flask, request, jsonify
import serial
import json
import struct
from ctypes import c_uint8

app = Flask(__name__)

API_KEYS = {
    "object_detection_system": "ods_key",
    "robot_software": "rs_key"
}

# Define the endpoint to receive data
@app.route('/write', methods=['POST'])
def write_to_file():
    try:
        # Initialize UART
        ser = serial.Serial('/dev/serial0', baudrate=115200, timeout=1)

        if ser.is_open:
            print("Serial connection established successfully.")

        else:
            print("Failed to open serial port.")

        # Get JSON data from the request
        data = request.get_json()   # Python dictionary
        if not isinstance(data, dict):
            return {"error": "Expected a dictionary"}, 400
        
        commands = data.get('message')
        if not isinstance(commands, list):
            return {"error": "Expected a list for 'message'"}, 400
        

        # Actual code starts here

        if commands[0] == "MoveCan":
            with open('updated_can_state.txt', 'r') as can_file:
                content = can_file.read()
            if "e_canInWorkspace" not in content:
                return {'status': 'error', 'message': 'Can is not in the workspace'}, 500
        
        formatted_commands = [c_uint8(element) for element in commands]
        
        # Write data to a text file to track most recent changes
        with open('received_data.txt', 'w') as file:
            for command in commands:
                file.write(str(command) + " - type " + str(type(command)) + '\n')

        # Fix sized packet created to be sent to MCU
        packet = struct.pack('BBB', formatted_commands[0].value, formatted_commands[1].value, formatted_commands[2].value)
        print_packet = list(packet)
        print(print_packet)

        # Send serial data to microcontroller (18 bytes total)
        ser.write(packet)

        # # Indicate that state of the can should be updated
        # with open('updated_can_state.txt', 'w') as can_file:
        #     can_file.write("Update" + '\n')
        
        # Close serial connection
        ser.close()

        return {'status': 'success', 'message': 'Data written to file'}, 200
    except Exception as e:
        return {'status': 'error', 'message': str(e)}, 500
    
# Define the endpoint to transmit data
@app.route('/read', methods=['GET'])
def read_from_file():
    
    # Check that API key matches one of the values in dictionary above
    api_key = request.headers.get('API-Key')
    if api_key not in API_KEYS.values():
        return jsonify({'status': 'error', 'message': 'Unauthorized'}), 401

    if api_key == API_KEYS["object_detection_system"]:
        try:
            with open('updated_can_state.txt', 'r+') as can_file:
                state = can_file.read()

                # Reset the file back to not updated
                can_file.seek(0)
                can_file.truncate(0)
                can_file.write("No update" + '\n')

                return jsonify({'status': 'success', 'data': state}), 200
        except FileNotFoundError:
            return {'status': 'error', 'message': 'File not found'}, 404
        except Exception as e:
            return {'status': 'error', 'message': str(e)}, 500

    elif api_key == API_KEYS["robot_software"]:
        try:
            # Read data from the text file
            with open('received_data.txt', 'r') as file:
                lines = file.readlines()
            
            # Convert lines into a JSON response
            return jsonify({'status': 'success', 'data': lines}), 200
        except FileNotFoundError:
            return {'status': 'error', 'message': 'File not found'}, 404
        except Exception as e:
            return {'status': 'error', 'message': str(e)}, 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)