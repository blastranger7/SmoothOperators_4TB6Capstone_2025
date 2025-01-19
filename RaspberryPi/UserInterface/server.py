from flask import Flask, request

app = Flask(__name__)

# Define the endpoint to receive data
@app.route('/write', methods=['POST'])
def write_to_file():
    try:
        # Get JSON data from the request
        data = request.json
        message = data.get('message', 'No message provided')
        
        # Write data to a text file
        with open('received_data.txt', 'a') as file:
            file.write(message + '\n')
        
        return {'status': 'success', 'message': 'Data written to file'}, 200
    except Exception as e:
        return {'status': 'error', 'message': str(e)}, 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)