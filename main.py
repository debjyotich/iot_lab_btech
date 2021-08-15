from flask import Flask, render_template
from flask_restful import reqparse
import datetime
import pymongo

app = Flask(__name__)

sensor_db = pymongo.MongoClient("mongodb://localhost:27017/")

db_connection = sensor_db['iot_server']  # database name
db_collection = db_connection['sensor_data']  # collection name


@app.route('/')
def index():
    return render_template('index.html')


# REST API for sending iot sensor data
@app.route('/update', methods=['GET'])
def update():
    parser = reqparse.RequestParser()
    parser.add_argument('api_key', type=str)
    parser.add_argument('field', type=float)
    data = parser.parse_args()
    data['date'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    db_collection.insert_one(data)  # store sensor data in MongoDB
    return '200 OK'


@app.route('/feeds', methods=['GET'])
def feeds():
    parser = reqparse.RequestParser()
    parser.add_argument('api_key', type=str)
    data = parser.parse_args()

    reply_from_db = [i for i in db_collection.find(data)][-1]
    reply_to_user = {"data": reply_from_db['field'], "date": reply_from_db['date']}
    return reply_to_user


if __name__ == '__main__':
    app.run(host='0.0.0.0')
