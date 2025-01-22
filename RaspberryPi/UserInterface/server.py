from flask import Flask, request, jsonify
import serial
import json

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
        ser = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

        # Get JSON data from the request
        data = request.get_json()   # Python dictionary
        if not isinstance(data, dict):
            return {"error": "Expected a dictionary"}, 400
        
        commands = data.get('message')
        if not isinstance(commands, list):
            return {"error": "Expected a list for 'message'"}, 400
        
        # Write data to a text file to track most recent changes
        with open('received_data.txt', 'w') as file:
            for command in commands:
                file.write(command + '\n')

        byte_objects = [obj.encode('utf-8') for obj in commands]    # Use python generator to convert every item in list to byteobject

        # Send serial data to microcontroller
        for byte_object in byte_objects:
            ser.write(byte_object)
            ser.write(b'\n')  # Not necessary: Add a newline character as a delimiter
            print(f"Sent: {byte_object}")

        ser.write(('#').encode('utf-8'))

        # Indicate that state of the can should be updated
        with open('updated_can_state.txt', 'w') as can_file:
            can_file.write("Update" + '\n')
        
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