from typing import Optional, List
from fastapi import FastAPI
from pydantic import BaseModel
from pymongo import MongoClient

app = FastAPI()


class Item(BaseModel):
    name: str
    price: float
    is_offer: Optional[bool] = None


class Snapshot(BaseModel):
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

    return {}


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

