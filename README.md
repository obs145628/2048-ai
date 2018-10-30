# 2048 AI implementation


2048 AI. 2048 grids are represented with 64 bits integers and operations are done using table in order to maximixe perfomance.

## Compilation

```shell
git submodule init
git submodule sync
git submodule update
python3 -m venv env
. env/bin/activate
pip install -r requirements.txt
mkdir _build
cd _build
cmake ..
make
```

The python environement is for the test suite and the ML implementations


## Usage
./2048-ai <mode> <agent>
mode:
- gui : play one game and display the game in the terminal
- stats: play an infinite number of games. Doesn' display the games, but display severals informaitons

agent:
- minmax : Implementation of min-max + alpha-beta pruning + iterative deepening + cache
- expectimax : Implementation of Expectiminmax + iterative deepening + cache


## Test suite

Python 3 is required to run the test suite

```

make check
```

## Supervised learning

Train a model with online supervised learning techniques:
```shell
make sl_online_ai
```

Evaluate a trained model:
```shell
MODEL=./ai_model.joblib make ml_eval_model
```