import os

from fastapi import FastAPI, HTTPException
from sqlmodel import Field, Session, SQLModel, create_engine, select


class CarBase(SQLModel): # base model for the car, all the other models derive from it
    vendor: str
    model: str
    color: str


class Car(CarBase, table=True): # the model that is being used when creating the database file, id becomes mandatory
    id: int | None = Field(default=None, primary_key=True)


class CarCreate(CarBase): # no changes necessary to the base, but gets its own model to make it look better in the doc page
    pass


class CarPublic(CarBase): # after a car gets inserted into the database, it gets an id
    id: int


class CarUpdate(SQLModel): # we are only updating the color of the cars, nothing else
    color: str


sqlite_file_name = "CarDB.db"
sqlite_url = f"sqlite:///{sqlite_file_name}"

connect_args = {"check_same_thread": False}
engine = create_engine(sqlite_url, echo=False, connect_args=connect_args)

if not os.path.exists(sqlite_file_name): # create the database only when it is not already existing
    SQLModel.metadata.create_all(engine)

app = FastAPI()

@app.post("/cars/", response_model=CarPublic) # creating a new car
def create_car(car: CarCreate):
    with Session(engine) as session:
        db_car = Car.model_validate(car)
        session.add(db_car)
        session.commit()
        session.refresh(db_car)
        return db_car


@app.get("/cars/", response_model=list[CarPublic]) # retrieving all cars
def read_cars():
    with Session(engine) as session:
        cars = session.exec(select(Car)).all()
        return cars


@app.get("/cars/{car_id}", response_model=CarPublic) # retrieving specific car by id
def read_car(car_id: int):
    with Session(engine) as session:
        car = session.get(Car, car_id)
        if not car:
            raise HTTPException(status_code=404, detail="Car not found")
        return car


@app.patch("/cars/{car_id}", response_model=CarPublic) # updating specific car by id
def update_car(car_id: int, car: CarUpdate):
    with Session(engine) as session:
        db_car = session.get(Car, car_id)
        if not db_car:
            raise HTTPException(status_code=404, detail="Car not found")
        car_data = car.model_dump(exclude_unset=True)
        for key, value in car_data.items():
            setattr(db_car, key, value)
        session.add(db_car)
        session.commit()
        session.refresh(db_car)
        return db_car


@app.delete("/cars/{car_id}") # deleting specific car by id
def delete_car(car_id: int):
    with Session(engine) as session:
        car = session.get(Car, car_id)
        if not car:
            raise HTTPException(status_code=404, detail="Car not found")
        session.delete(car)
        session.commit()
        return {"OK": True}
