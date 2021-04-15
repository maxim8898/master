import pymongo
from typing import Optional, List
from datetime import datetime
from fastapi import FastAPI
from pydantic import BaseModel, Field
from pymongo import MongoClient

app = FastAPI()


class Item(BaseModel):
    name: str
    price: float
    is_offer: Optional[bool] = None


class Snapshot(BaseModel):
    datetime: Optional[datetime]
    left: int
    left_center: int
    right_center: int
    right: int
    speed: int
    temperature: int
    angle: int


class SnapshotList(BaseModel):
    each_item: List[Snapshot]


@app.post("/snapshots")
def push_data(item: Snapshot):
    uri = "mongodb://root:example@mongo:27017"
    client = MongoClient(uri)
    db = client['snapshots']
    collection = db['sensors']
    collection.insert_one(item.dict())

    return {"Message": "Collection successfully added", "collection": item.dict()}


@app.post("/snapshots/removeAll")
def remove_all_data():
    uri = "mongodb://root:example@mongo:27017"
    client = MongoClient(uri)
    db = client['snapshots']
    collection = db['sensors']
    result = collection.delete_many({})

    return {"Snapshots deleted": result.deleted_count}


@app.get("/snapshot")
def get_data():
    uri = "mongodb://root:example@mongo:27017"
    client = MongoClient(uri)
    db = client['snapshots']
    collection = db['sensors']
    items = collection.find()
    snap_list = []

    for item in items:
        snap_list.append(Snapshot(
            datetime=item['datetime'],
            left=item['left'],
            left_center=item['left_center'],
            right_center=item['right_center'],
            right=item['right'],
            speed=item['speed'],
            temperature=item['temperature'],
            angle=item['angle']
        ))
    snap_list = SnapshotList(each_item=snap_list)

    return {"collection": snap_list.dict()}


@app.get("/snapshot/topNRows")
def get_data(count_of_rows: int):
    uri = "mongodb://root:example@mongo:27017"
    client = MongoClient(uri)
    db = client['snapshots']
    collection = db['sensors']
    items = collection.find().sort('datetime', pymongo.DESCENDING).limit(count_of_rows)
    snap_list = []

    for item in items:
        snap_list.append(Snapshot(
            datetime=item['datetime'],
            left=item['left'],
            left_center=item['left_center'],
            right_center=item['right_center'],
            right=item['right'],
            speed=item['speed'],
            temperature=item['temperature'],
            angle=item['angle']
        ))
    snap_list = SnapshotList(each_item=snap_list)

    return {"collection": snap_list.dict()}
