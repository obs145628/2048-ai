import world
import sys

def task_gridtest():

    if len(sys.argv) < 4:
        print('gridtest: missing arguments', file = sys.stderr)
        return 1

    type = sys.argv[2]
    moves_file = sys.argv[3]

    with open(moves_file, 'r') as f:

        
        if type == 'world':
            w = world.World(4)
            w.play_actions(world.World.unserialize_actions(f))
            w.write_full(sys.stdout)
            return 0

        else:
            print("Unknow type: "  + type, file = sys.stderr)
            return 1


if __name__ == '__main__':

    if len(sys.argv) < 2:
        print('2048: missing task', file = sys.stderr)
        sys.exit(1)

    task = sys.argv[1]
    if task == 'gridtest':
        sys.exit(task_gridtest())

    else:
        print('2048: unknow task ' + task, file = sys.stderr)
        sys.exit(1)
