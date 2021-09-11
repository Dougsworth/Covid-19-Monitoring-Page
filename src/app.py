from flask import Flask, request, jsonify, render_template
from marshmallow import Schema, fields, ValidationError
from flask_pymongo import PyMongo
from flask_cors import CORS

from bson.json_util import dumps
from json import loads

username = "neddamj"
password = "Ja.sparky77"
app = Flask(__name__)
app.config['SECRET_KEY'] = 'hjevdjsvfvyejs'
app.config["MONGO_URI"] = "mongodb+srv://{}:{}@cluster0.weykq.mongodb.net/monday?retryWrites=true&w=majority".format(username, password)

mongo = PyMongo(app)
CORS(app)

class DataSchema(Schema):
    temperature = fields.Float(required=True)
    status = fields.String(required=True)
    year = fields.Integer(required=True)
    month = fields.Integer(required=True)
    day = fields.Integer(required=True)
    hour = fields.Integer(required=True)
    minute = fields.Integer(required=True)
    second = fields.Integer(required=True)

###################### APP #############################
@app.route('/')
def index():
    return render_template('index.html')

################# API ENDPOINTS ########################
@app.route('/api/data')
def return_table_data():
    table_data = mongo.db.datas.find()
    return jsonify(loads(dumps(table_data))) 

@app.route('/api/esp', methods=['POST'])
def test():
    def fever_or_not(temp):
        if temp >= 38.0:
            return "Abnormal"
        else:
            return "Normal"

    if request.method == 'POST':

        # Format the data into a dictionary
        embedded_data = {
            "temperature": request.json['temperature'],
            "status": fever_or_not(request.json['temperature']),
            "year": request.json['year'],
            "month": request.json['month'],
            "day": request.json['day'],
            "hour": request.json['hour'],
            "minute": request.json['minute'],
            "second": request.json['second']
        }

        # Upload the data to the database
        data = DataSchema().load(embedded_data)
        data_id = mongo.db.datas.insert_one(data).inserted_id
        embedded_data = mongo.db.datas.find_one(data)

        print(embedded_data)
        return jsonify(request.json)

if __name__ == "__main__":
    app.run(
        debug=True,
        host="192.168.1.10",
        port=5000
    )