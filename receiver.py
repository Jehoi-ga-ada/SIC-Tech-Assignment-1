from flask import Flask, request, jsonify, render_template_string

app = Flask(__name__)

# Variabel global untuk menyimpan angka terakhir yang diterima
last_humidity = None

@app.route('/post-data', methods=['POST'])
def post_data():
    global last_humidity
    data = request.get_json()
    humidity = data.get('humidity', None)
    
    if humidity is not None:
        last_humidity = humidity
        response = {
            "status": "success",
            "message": f"humidity received: {humidity}"
        }
        return jsonify(response), 200
    else:
        response = {
            "status": "error",
            "message": "No humidity provided"
        }
        return jsonify(response), 400

@app.route('/get-data', methods=['GET'])
def get_data():
    global last_humidity
    return jsonify({"humidity": last_humidity})

@app.route('/')
def index():
    html = """
    <!DOCTYPE html>
    <html>
    <head>
        <title>ESP32 Data</title>
        <script>
            function fetchData() {
                fetch('/get-data')
                    .then(response => response.json())
                    .then(data => {
                        document.getElementById('humidity').innerText = data.humidity;
                    });
            }
            setInterval(fetchData, 2000); // Fetch data every 2 seconds
        </script>
    </head>
    <body onload="fetchData()">
        <h1>Data from ESP32</h1>
        <p>Last humidity received: <span id="humidity">Loading...</span></p>
    </body>
    </html>
    """
    return render_template_string(html)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
