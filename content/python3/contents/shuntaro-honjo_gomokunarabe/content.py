import readchar

CUBE_SIZE = {
	'x': 16,
	'y': 32,
	'z': 8
}
KEY_MAP = {
	'k': 'up',
	'j': 'down',
	'l': 'right',
	'h': 'left',
	'i': 'forward',
	'm': 'backward'
}
STONE_COLOR = [0xFF0000, 0x0000FF]
CURSOR_COLOR = [0xFFAAAA, 0xAAAAFF]
PLAYER_NAME = ['RED', 'BLUE']


def plus_list(list1, list2):
	return [i + j for (i, j) in zip(list1, list2)]


def scalar_mul(list1, coff):
	return [i * coff for i in list1]


class GoBoard:
	def __init__(self, x, y, z):
		self.size = (x, y, z)
		self.stones = [[[-1 for i in range(z)] for j in range(y)] for k in range(x)]

	def putStone(self, x, y, z, playerNo):
		if self.stones[x][y][z] != -1:
			return False
		else:
			self.stones[x][y][z] = playerNo
			return True

	def judge(self):
		for x in range(self.size[0]):
			for y in range(self.size[1]):
				for z in range(self.size[2]):
					if self.__isConnected5Stones(x, y, z):
						return self.stones[x][y][z]
					else:
						continue
		return -1

	def __isConnected5Stones(self, x, y, z):
		directions = [
			[1, 0, 0],
			[0, 1, 0],
			[1, 1, 0],
			[0, 0, 1],
			[1, 0, 1],
			[0, 1, 1],
			[1, 1, 1]
		]

		playerNo = self.stones[x][y][z]

		if playerNo == -1:
			return False

		for d in directions:
			count = 1

			tmp = [x, y, z]
			while True:
				tmp = plus_list(tmp, d)
				if self.__isOutOfBoard(tmp[0], tmp[1], tmp[2]):
					break

				if self.stones[tmp[0]][tmp[1]][tmp[2]] == playerNo:
					count = count + 1
				else:
					break

			tmp = [x, y, z]
			while True:
				tmp = plus_list(tmp, scalar_mul(d, -1))
				if self.__isOutOfBoard(tmp[0], tmp[1], tmp[2]):
					break

				if self.stones[tmp[0]][tmp[1]][tmp[2]] == playerNo:
					count = count + 1
				else:
					break

			if count >= 5:
				return True

		return False

	def __isOutOfBoard(self, x, y, z):
		if x < 0 or x >= self.size[0]:
			return True
		elif y < 0 or y >= self.size[1]:
			return True
		elif z < 0 or y >= self.size[2]:
			return True

		return False


class Cursor:
	def __init__(self):
		self.x = 0
		self.y = 0
		self.z = 0
		self.color = 0xFFFFFF

	def move(self, direction, step=1):
		if direction == 'down':
			self.y = (self.y + step) % CUBE_SIZE['y']
		elif direction == 'up':
			self.y = (self.y - step) % CUBE_SIZE['y']
		elif direction == 'right':
			self.x = (self.x + step) % CUBE_SIZE['x']
		elif direction == 'left':
			self.x = (self.x - step) % CUBE_SIZE['x']
		elif direction == 'forward':
			self.z = (self.z + step) % CUBE_SIZE['z']
		elif direction == 'backward':
			self.z = (self.z - step) % CUBE_SIZE['z']
		else:
			pass

	def moveTo(self, x, y, z):
		if x < 0 or x >= CUBE_SIZE['x']:
			return
		elif y < 0 or y >= CUBE_SIZE['y']:
			return
		elif z < 0 or z >= CUBE_SIZE['z']:
			return
		else:
			self.x = x
			self.y = y
			self.z = z

	def changeColor(self, color):
		self.color = color


def execute(led):
	goBoard = GoBoard(CUBE_SIZE['x'], CUBE_SIZE['y'], CUBE_SIZE['z'])
	cursor = Cursor()
	playerNo = 0

	printHowToPlay()

	cursor.moveTo(0, 0, 0)
	cursor.changeColor(CURSOR_COLOR[playerNo])

	updateLED(led, cursor, goBoard)

	while True:
		key = readchar.readchar()
		# print(key)
		if key == 'q':
			break
		elif key in KEY_MAP:
			cursor.move(KEY_MAP[key])
		elif key == 'x':
			isSucceeded = goBoard.putStone(cursor.x, cursor.y, cursor.z, playerNo)
			if not isSucceeded:
				continue

			result = goBoard.judge()
			if result == -1:
				playerNo = (playerNo + 1) % 2
				cursor.moveTo(0, 0, 0)
				cursor.changeColor(CURSOR_COLOR[playerNo])
			else:
				print('Player %s win!' % PLAYER_NAME[result])
				winLED(led, result)

		updateLED(led, cursor, goBoard)


def updateLED(led, cursor, goBoard):
	led.Clear()
	for x in range(goBoard.size[0]):
		for y in range(goBoard.size[1]):
			for z in range(goBoard.size[2]):
				if goBoard.stones[x][y][z] == -1:
					continue

				led.SetLed(x, y, z, STONE_COLOR[goBoard.stones[x][y][z]])

	led.SetLed(cursor.x, cursor.y, cursor.z, cursor.color)

	led.Show()


def winLED(led, winnerNo):
	color = 0x000000
	led.Wait(1000)

	while True:
		if color == 0x000000:
			color = STONE_COLOR[winnerNo]
		else:
			color = 0x000000

		for x in range(CUBE_SIZE['x']):
			for y in range(CUBE_SIZE['y']):
				for z in range(CUBE_SIZE['z']):
					led.SetLed(x, y, z, color)
			led.Show()
			led.Wait(100)


def printHowToPlay():
	print('HOW TO PLAY "3D GOMOKU NARABE"')
	print('===============')
	print('Type following key to play.')
	print('')
	print('h/j/k/l/i/m: MOVE CURSOR')
	print('x: PUT STONE')
	print('q: EXIT')
	print('')
	print('Enjoy the game!')
