from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
from typing import List, Tuple
import mec_lib
import time

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class PointsInput(BaseModel):
    points: List[Tuple[float, float]]

# This tells FastAPI to serve your index.html when you visit the main Render URL
@app.get("/", response_class=HTMLResponse)
async def serve_ui():
    with open("index.html", "r", encoding="utf-8") as f:
        return f.read()

@app.post("/calculate")
async def calculate(data: PointsInput):
    start_time = time.perf_counter()
    result = mec_lib.compute_mec(data.points)
    end_time = time.perf_counter()

    return {
        "center_x": result.x,
        "center_y": result.y,
        "radius": result.r,
        "time_ms": (end_time - start_time) * 1000
    }
