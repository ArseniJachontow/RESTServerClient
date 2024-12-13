import os

from fastapi import FastAPI, HTTPException, Query
from sqlmodel import Field, Session, SQLModel, create_engine, select


class CarBase(SQLModel):
    vendor: str
    model: str
    color: str


class Car(CarBase, table=True):
    id: int | None = Field(default=None, primary_key=True)


class CarCreate(CarBase):
    pass


class CarPublic(CarBase):
    id: int


class CarUpdate(SQLModel):
    color: str


sqlite_file_name = "CarDB.db"
sqlite_url = f"sqlite:///{sqlite_file_name}"

connect_args = {"check_same_thread": False}
engine = create_engine(sqlite_url, echo=False, connect_args=connect_args)

if not os.path.exists(sqlite_file_name):
    SQLModel.metadata.create_all(engine)

app = FastAPI()

@app.post("/cars/", response_model=CarPublic)
def create_car(car: CarCreate):
    with Session(engine) as session:
        db_car = Car.model_validate(car)
        session.add(db_car)
        session.commit()
        session.refresh(db_car)
        return db_car


@app.get("/cars/", response_model=list[CarPublic])
def read_cars():
    with Session(engine) as session:
        cars = session.exec(select(Car)).all()
        return cars


@app.get("/cars/{car_id}", response_model=CarPublic)
def read_car(car_id: int):
    with Session(engine) as session:
        car = session.get(Car, car_id)
        if not car:
            raise HTTPException(status_code=404, detail="Car not found")
        return car


@app.patch("/cars/{car_id}", response_model=CarPublic)
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


@app.delete("/cars/{car_id}")
def delete_car(car_id: int):
    with Session(engine) as session:
        car = session.get(Car, car_id)
        if not car:
            raise HTTPException(status_code=404, detail="Car not found")
        session.delete(car)
        session.commit()
        return {"OK": True}
    