from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/test', methods=['POST'])
def test():
    if request.method == 'POST':
        print(request.json['temperature'], request.json['year'], request.json['month'], request.json['day'], request.json['hour'], request.json['minute'], request.json['second'])

        return jsonify(request.json)

if __name__ == "__main__":
    app.run(
        debug=True,
        host="192.168.1.12",
        port=5000
    )