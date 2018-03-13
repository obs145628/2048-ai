import env2048
import sys


def dump(grid):
    for i in range(16):
        sys.stdout.write(str(grid.get_cell1(i)) + ' ')
    sys.stdout.write(str(grid.score) + '\n')

def unserialize_actions(iis):
    res = []
    for l in iis.readlines():
        for w in l.split(' '):
            res.append(int(w))
    return res

def play_actions(actions):

    grid = env2048.Grid()
    grid.set_cell(actions[0], actions[1])
    sys.stdout.write(str(actions[0]) + ' ' + str(actions[1]) + '\n')
    dump(grid)
    grid.set_cell(actions[2], actions[3])
    sys.stdout.write(str(actions[2]) + ' ' + str(actions[3]) + '\n')
    dump(grid)

    for i in range(4, len(actions), 3):
        grid.take_action(actions[i])
        grid.set_cell(actions[i + 1], actions[i + 2])
        sys.stdout.write(str(actions[i]) + ' ' + str(actions[i + 1]) + ' '
                             + str(actions[i + 2]) + '\n')
        dump(grid)

    

    assert grid.finished()
    
'''
def write_full(self, os):
        copy = World(self.size)

        copy.add_num(self.actions[0], self.actions[1])
        os.write(str(self.actions[0]) + ' ' + str(self.actions[1]) + '\n')
        copy.serialize(os)
        os.write('\n')
        copy.add_num(self.actions[2], self.actions[3])
        os.write(str(self.actions[2]) + ' ' + str(self.actions[3]) + '\n')
        copy.serialize(os)
        os.write('\n')

        for i in range(4, len(self.actions), 3):
            copy.take_action(self.actions[i])
            copy.add_num(self.actions[i + 1], self.actions[i + 2])
            os.write(str(self.actions[i]) + ' ' + str(self.actions[i + 1]) + ' '
                     + str(self.actions[i + 2]) + '\n')
            copy.serialize(os)
            os.write('\n')
'''
    

def task_gridtest():

    if len(sys.argv) < 4:
        print('gridtest: missing arguments', file = sys.stderr)
        return 1

    type = sys.argv[2]
    moves_file = sys.argv[3]

    with open(moves_file, 'r') as f:

        
        if type == 'world':
            play_actions(unserialize_actions(f))
            #w.write_full(sys.stdout)
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

print('ok')
