import random
import gui


GRID_SIZE = 600
MARGIN = 20
FONT_SIZE = 150
BORDER_COLOR = (187, 173,  160)


BG_COLORS = [
    (205, 192, 180), #empty
    (238, 228, 218), #2
    (237, 224, 200), #4
    (242, 177, 121), #8
    (242, 177, 121), #16
    (246, 124, 95), #32
    (246, 94, 59), #64
    (237, 207, 114), #128
    (237, 204, 97), #256
    (237, 200, 80), #512
    (237, 197, 63), #1024
    (237, 194, 46), #2048
    (0, 0, 0), #4096
    (0, 0, 0), #8192
    (0, 0, 0), #16384
    (0, 0, 0), #32768
    (0, 0, 0), #65536
    (0, 0, 0), #131072
]

FG_COLORS = [
    (205, 192, 180), #empty
    (119, 110, 101), #2
    (119, 110, 101), #4
    (255, 255, 255), #8
    (255, 255, 255), #16
    (255, 255, 255), #32
    (255, 255, 255), #64
    (255, 255, 255), #128
    (255, 255, 255), #256
    (255, 255, 255), #512
    (255, 255, 255), #1024
    (255, 255, 255), #2048
    (255, 255, 255), #4096
    (255, 255, 255), #8192
    (255, 255, 255), #16384
    (255, 255, 255), #32768
    (255, 255, 255), #65536
    (255, 255, 255), #131072
]

ACTION_LEFT = 0
ACTION_RIGHT = 1
ACTION_UP = 2
ACTION_DOWN = 3
ACTION_NO = 4

def pow2(n):
    res = 1
    for _ in range(n):
        res *= 2
    return res

class World:

    @staticmethod
    def serialize_actions(actions, os):
        os.write(str(actions[0]) + ' ' + str(actions[1]) + '\n')
        os.write(str(actions[2]) + ' ' + str(actions[3]) + '\n')
        for i in range(4, len(actions), 3):
            os.write(str(actions[i]) + ' ' + str(actions[i + 1])
                     + ' ' + str(actions[i + 2]) + '\n')

    @staticmethod
    def unserialize_actions(iis):
        res = []
        for l in iis.readlines():
            for w in l.split(' '):
                res.append(int(w))
        return res
    

    def __init__(self, size):
        self.size = size
        self.reset()

    def get_cell(self, row, col):
        return self.data[row * self.size + col]

    def set_cell(self, row, col, val):
        self.data[row * self.size + col] = val


    def reset(self):
        self.data = [0] * self.size * self.size
        self.score = 0
        self.delta = 0
        self.actions = []

    def add_rand(self):

        val = 1
        if random.randint(0, 9) == 0:
            val = 2
        pos = 0

        while True:
            pos = random.randint(0, self.size * self.size - 1)
            if self.data[pos] != 0:
                continue
            self.data[pos] = val
            break

        self.actions.append(pos)
        self.actions.append(val)

    def add_num(self, pos, val):
        self.data[pos] = val
        self.actions.append(pos)
        self.actions.append(val)


    def render_gui(self):

        n = self.size
        square_size = int(GRID_SIZE / n)
        grid_size = int(square_size * n)

        for i in range(n):
            for j in range(n):
                val = self.get_cell(i, j)
                x = MARGIN + j * square_size
                y = MARGIN + i * square_size

                gui.fill_rect(x, y, square_size, square_size,
                              BG_COLORS[val])

                sval = str(pow2(val))
                size = gui.get_text_size('Arial', FONT_SIZE / n, sval)
                
                gui.draw_text(x + (square_size - size[0]) / 2,
                              y + (square_size - size[1]) / 2,
                              sval,
                              FG_COLORS[val], 'Arial',
                              FONT_SIZE / n)

    def render_cli(self):
        #TODO
        return

    def move_left(self):
        n = self.size
        reward = 0

        for i in range(0, n):

            level = 0
            can_fusion = False
            while level < n:
                j = level
                while j < n and self.get_cell(i, j) == 0:
                    j += 1
                if j == n:
                    break

                val = self.get_cell(i, j)
                self.set_cell(i, j, 0)

                if can_fusion and val == self.get_cell(i, level - 1):
                    self.set_cell(i, level - 1, val + 1)
                    reward += pow2(val + 1)
                    can_fusion = False

                else:
                    self.set_cell(i, level, val)
                    can_fusion = True
                    level += 1

        return reward

    def move_right(self):

        n = self.size
        reward = 0

        for i in range(0, n):

            level = n - 1
            can_fusion = False
            while level >= 0:
                j = level
                while j >= 0 and self.get_cell(i, j) == 0:
                    j -= 1
                if j == -1:
                    break

                val = self.get_cell(i, j)
                self.set_cell(i, j, 0)

                if can_fusion and val == self.get_cell(i, level + 1):
                    self.set_cell(i, level + 1, val + 1)
                    reward += pow2(val + 1)
                    can_fusion = False

                else:
                    self.set_cell(i, level, val)
                    can_fusion = True
                    level -= 1

        return reward

    def move_up(self):

        n = self.size
        reward = 0

        for i in range(0, n):

            level = 0
            can_fusion = False
            while level < n:
                j = level
                while j < n and self.get_cell(j, i) == 0:
                    j += 1
                if j == n:
                    break

                val = self.get_cell(j, i)
                self.set_cell(j, i, 0)

                if can_fusion and val == self.get_cell(level - 1, i):
                    self.set_cell(level - 1, i, val + 1)
                    reward += pow2(val + 1)
                    can_fusion = False

                else:
                    self.set_cell(level, i, val)
                    can_fusion = True
                    level += 1

        return reward

    def move_down(self):

        n = self.size
        reward = 0

        for i in range(0, n):

            level = n - 1
            can_fusion = False
            while level >= 0:
                j = level
                while j >= 0 and self.get_cell(j, i) == 0:
                    j -= 1
                if j == -1:
                    break

                val = self.get_cell(j, i)
                self.set_cell(j, i, 0)

                if can_fusion and val == self.get_cell(level + 1, i):
                    self.set_cell(level + 1, i, val + 1)
                    reward += pow2(val + 1)
                    can_fusion = False

                else:
                    self.set_cell(level, i, val)
                    can_fusion = True
                    level -= 1

        return reward

    def take_action(self, action):

        reward = 0
        if action == ACTION_LEFT:
            reward = self.move_left()
        elif action == ACTION_RIGHT:
            reward = self.move_right()
        elif action == ACTION_UP:
              reward = self.move_up()
        else:
            reward = self.move_down()

        self.score += reward
        self.actions.append(action)
        return reward

    def play_actions(self, actions):
              
        self.reset()
        self.add_num(actions[0], actions[1])
        self.add_num(actions[2], actions[3])

        for i in range(4, len(actions), 3):
            self.take_action(actions[i])
            self.add_num(actions[i + 1], actions[i + 2])

    def is_action_valid(self, action):      
        copy = World(self.size)
        copy.data = self.data[:]
        data.take_action(action)
        return self.data != action.data

    def is_finished(self):

        for a in range(0, 4):
            if self.is_action_valid(a):
                return False
        return True

    def serialize(self, os):
        for i in range(self.size * self.size):
              os.write(str(self.data[i]) + ' ')
        os.write(str(self.score))

    def write_actions(self, os):
        serialize_actions(self.actions, os)

    def read_actions(self, iss):
        self.play_actions(unserialize_actions(iss))      

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


    def flip(self):
        data = [0] * self.size * self.size
        for i in range(self.size):
            for j in range(self.size):
                data[i * self.size + j] = self.data[i * self.size + self.size - 1 - j]

        self.data = data

    def transpose(self):
        data = [0] * self.size * self.size
        for i in range(self.size):
            for j in range(self.size):
                data[i * self.size + j] = self.data[j * self.size + i]

        self.data = data

