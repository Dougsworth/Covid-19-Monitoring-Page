from flask import Flask, request, jsonify, render_template
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

###################### APP #############################
@app.route('/')
def index():
    return render_template('index.html')

################# API ENDPOINTS ########################
@app.route('/test', methods=['POST'])
def test():
    if request.method == 'POST':

        embedded_data = {
            "temperature": request.json['temperature'],
            "year": request.json['year'],
            "month": request.json['month'],
            "day": request.json['day'],
            "hour": request.json['hour'],
            "minute": request.json['minute'],
            "second": request.json['second']
        }

        print(embedded_data)

        return jsonify(request.json)

if __name__ == "__main__":
    app.run(
        debug=True,
        host="192.168.1.10",
        port=5000
    )