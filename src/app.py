from flask import Flask, request, jsonify, render_template, redirect, url_for, session, flash
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

class AuthenticationSchema(Schema):
    username = fields.String(required=True)
    password = fields.String(required=True)

class LocationSchema(Schema):
    location = fields.String(required=True)
    id = fields.String(required=True)

class DataSchema(Schema):
    temperature = fields.Float(required=True)
    status = fields.String(required=True)
    year = fields.Integer(required=True)
    month = fields.Integer(required=True)
    day = fields.Integer(required=True)
    hour = fields.Integer(required=True)
    minute = fields.Integer(required=True)
    second = fields.Integer(required=True)
    id = fields.String(required=True)

###################### APP #############################
@app.route('/')
def home():
    return redirect(url_for('login'))

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == "POST":
        try:
            # Check to see if the username is in the database already
            u_name = request.form['username']
            auth = mongo.db.authentications.find_one({"username": u_name})
            if auth is not None:
                flash("Username is already taken", "info")
                return render_template('signup.html')         
            else:
                # Store the username and password in a database, then redirect to 
                # the login page
                auth_data =  AuthenticationSchema().load(request.form)
                print(auth_data)
                mongo.db.authentications.insert_one(auth_data)
                return redirect(url_for('login'))
        except ValidationError as e:
            return e.messages, 400
    else:
        return render_template('signup.html')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        try:
            u_name = request.form["username"]
            p_word = request.form["password"]
            print(u_name, p_word)

            try:
                auth = mongo.db.authentications.find_one({"username": u_name})

                if p_word == auth['password']:
                    print('Correct information')
                    session['user'] = u_name
                    return redirect(url_for('index'))
                else:
                    # Incorrect password
                    print('Incorrect password')
                    flash('The password you entered was incorrect', "info")
                    return render_template('login.html')
            except:
                # Incorrect username
                print('Incorrect username')
                flash('The username you entered was incorrect', "info")
                return render_template('login.html')
        except ValidationError as e:
            return e.messages, 400
    else:
        if "user" in session:
            return redirect(url_for('index'))

        return render_template('login.html')

@app.route('/logout')
def logout():
    session.pop('user', None)
    return redirect(url_for('login'))

@app.route('/index')
def index():
    return render_template('index.html')

@app.route('/location', methods=['GET', 'POST'])
def location():
    if request.method == 'POST':
        location_data = {
            "location": request.form['location'],
            "id": request.form['id']
        }

        # Upload the data to the database
        data = LocationSchema().load(location_data)
        location_id = mongo.db.locations.insert_one(data).inserted_id
        location_data = mongo.db.locations.find_one(data)

        return render_template('location.html')
    else:
        return render_template('location.html')
    
################# API ENDPOINTS ########################
@app.route('/api/data')
def return_table_data():
    table_data = mongo.db.datas.find()
    return jsonify(loads(dumps(table_data))) 
    

@app.route('/api/location/<id>')
def get_location_data(id):
    location_data = mongo.db.locations.find({"id": id})
    return jsonify(loads(dumps(location_data)))

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
            "second": request.json['second'],
            "id": request.json['id']
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
        host="192.168.1.8",
        port=5000
    )