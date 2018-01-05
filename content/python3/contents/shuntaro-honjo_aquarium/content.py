import random

FPS = 10
CUBE_SIZE = {
	'w': 16,
	'h': 32,
	'd': 8
}
objs = []


class Sprite:
	def __init__(self, x, y, z, image):
		self.x = x
		self.y = y
		self.z = z
		self.image = image

	def draw(self, led):
		for x in range(len(self.image)):
			for y in range(len(self.image[0])):
				for z in range(len(self.image[0][0])):
					if self.image[x][y][z] == -1:
						continue
					elif self.x + x < 0 or self.x + x >= CUBE_SIZE['w']:
						continue
					elif self.y + y < 0 or self.y + y >= CUBE_SIZE['h']:
						continue
					elif self.z + y < 0 or self.z + z >= CUBE_SIZE['d']:
						continue
					else:
						led.SetLed(self.x + x, self.y + y, self.z + z, self.image[x][y][z])

	def isOutOfCube(self):
		if self.x < 0 or self.x >= CUBE_SIZE['w']:
			return True
		elif self.y < 0 or self.y >= CUBE_SIZE['h']:
			return True
		elif self.z < 0 or self.z >= CUBE_SIZE['d']:
			return True

		return False

	def delete(self):
		objs.remove(self)


class Sea(Sprite):
	def __init__(self):
		image = []
		for x in range(CUBE_SIZE['w']):
			image.append([])
			for y in range(CUBE_SIZE['h']):
				image[x].append([])
				for z in range(CUBE_SIZE['d']):
					image[x][y].append(0x99BBEE - 0x040401 * y)

		super().__init__(0, 0, 0, image)

	def update(self):
		pass


class Bouble(Sprite):
	def __init__(self, x, y, z):
		image = [[[0xEEEEFF]]]
		super().__init__(x, y, z, image)

	def update(self):
		self.y = self.y - 1


class Fish(Sprite):
	def __init__(self, x, y, z):
		shape = [
			[1, 0, 1, 1, 0],
			[1, 1, 1, 1, 1],
			[1, 0, 1, 1, 0]
		]

		color = int(0xDDDDFF * (1 - random.random() / 2))

		image = []
		for i in range(len(shape[0])):
			image.append([])
			for j in range(len(shape)):
				if shape[j][i] == 1:
					image[i].append([color])
				else:
					image[i].append([-1])

		super().__init__(x, y, z, image)

	def update(self):
		self.x = self.x + int(random.random() * 2)


class Kelp(Sprite):
	def __init__(self, x, z):
		shape = [
			[0, 1, 1],
			[1, 1, 0],
			[1, 1, 0],
			[0, 1, 1],
			[0, 1, 1],
			[1, 1, 0]
		]

		image = []
		for i in range(len(shape[0])):
			image.append([])
			for j in range(len(shape)):
				if shape[j][i] == 1:
					image[i].append([0x55DD88])
				else:
					image[i].append([-1])

		super().__init__(x, CUBE_SIZE['h'] - len(image[0]), z, image)

	def update(self):
		pass


class Ground(Sprite):
	def __init__(self):
		image = [[[0x554422] * CUBE_SIZE['d']] * 1] * CUBE_SIZE['w']
		super().__init__(0, CUBE_SIZE['h'] - 1, 0, image)

	def update(self):
		pass


def init():
	sea = Sea()
	ground = Ground()
	kelp1 = Kelp(3, 2)
	kelp2 = Kelp(10, 4)

	objs.append(sea)
	objs.append(ground)
	objs.append(kelp1)
	objs.append(kelp2)


def generateBouble():
	x = int(random.random() * CUBE_SIZE['w'])
	y = CUBE_SIZE['h'] - 1
	z = int(random.random() * CUBE_SIZE['d'])
	bouble = Bouble(x, y, z)
	objs.append(bouble)


def generateFish():
	x = 0
	y = int(random.random() * CUBE_SIZE['h'])
	z = int(random.random() * CUBE_SIZE['d'])
	fish = Fish(x, y, z)
	objs.append(fish)


def update():
	for obj in objs:
		obj.update()
		if obj.isOutOfCube():
			objs.remove(obj)


def draw(led):
	for obj in objs:
		obj.draw(led)
	led.Show()


def execute(led):
	init()

	while True:
		if random.random() > 0.8:
			generateBouble()
		if random.random() > 0.925:
			generateFish()
		update()
		draw(led)
		led.Wait(int(FPS / 60 * 1000))
