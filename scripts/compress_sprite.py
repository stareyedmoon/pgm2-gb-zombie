#!/usr/bin/env python3

# Don't read this code. I hacked it together in half an hour.
# If you wanna know what it does just look at the Pokemon Gen1 sprite compression algorithm.

import math
import sys

from PIL import Image

from typing_extensions import Self, Literal

class Bits:
	_bits: int
	_length: int
	_seek: int

	def __init__(self, bits: int = 0, length: int = 0):
		self._bits = bits & ((1 << length) - 1)
		self._length = length
		self._seek = 0

	def __len__(self) -> int:
		return self._length
	
	def __xor__(self: Self, other: Self) -> Self:
		tlen = min(len(self), len(other))
		return Bits(self._bits ^ other._bits, tlen) 

	def __getitem__(self, key: int) -> bool:
		if key < 0:
			key = self._length + key
		if key < 0 or key >= self._length:
			raise IndexError(f"Index {key} out of range.")
		
		return ((self._bits >> (self._length - key - 1)) & 1) == 1

	def __setitem__(self, key: int, value: bool):
		if key < 0:
			key = self._length + key
		if key < 0 or key >= self._length:
			raise IndexError(f"Index {key} out of range.")
		
		bit = 1 << (self._length - key - 1)
		if value:
			self._bits |= bit
		else:
			self._bits &= ~bit

	def tell(self) -> int:
		return self._seek;

	def seek(self, position: int, rel: Literal["start", "relative", "end"] = "start"):
		match rel:
			case "start":
				self._seek = position
			case "relative":
				self._seek += position
			case "end":
				self._seek = self._length + position
		
		self._seek = min(max(self._seek, 0), self._length)

	def read(self, length: int) -> tuple[int, int]:
		length = min(length, self._length - self._seek)
		bits = (self._bits >> (self._length - self._seek - 1)) & ((1 << length) - 1)
		self._seek += length
		return (bits, length)

	def push(self, bits: int | tuple[int, int] | Self, length: int | None = None):
		#print(f"Bits.push(\n\t{bits},\n\t{length}\n)")

		if type(bits) == int:
			if type(length) != int:
				raise ValueError(f"Invalid length for Bits.push(int) '{length}'")
			self._bits <<= length
			self._bits |= bits & ((1 << length) - 1)
			self._length += length
		elif type(bits) == tuple[int, int] or type(bits) == tuple:
			self.push(bits[0], bits[1])
		else:
			# Assume it's a Bits instance. If it isn't we'll just burn.
			self.push(bits._bits, bits._length)

	def to_bytes(self) -> bytes:
		bits = self._bits
		length = self._length

		delta = (8 - (length % 8)) % 8

		bits <<= delta
		length += delta

		return bits.to_bytes(length // 8, "big")

def image_get_pixel(image: Image.Image, x: int, y: int) -> tuple[int, int, int]:
	pixel = image.getpixel((x, y))

	match image.mode:
		case "1":
			return (255,255,255) if pixel else (0,0,0)
		case "L":
			return (pixel,pixel,pixel)
		case "P":
			return tuple(image.getpalette()[pixel*3:pixel*3+2])
		case "RGB":
			return pixel
		case "RGBA":
			return (pixel[0], pixel[1], pixel[2])
		case _:
			raise AttributeError(f"Unsupported image mode {image.mode}.")

def get_closest_color_from_palette(color: tuple[int, int, int]) -> int:
	PALETTE = [
		( 60,  79,   0),
		( 72, 100,   8),
		( 96, 116,   4),
		(145, 155,   6)
	]

	best_dist = 1e100
	best_col_index = -1
	for i in range(len(PALETTE)):
		cur_dist = sum([((c[0]-c[1])**2) for c in zip(PALETTE[i], color)])
		if cur_dist < best_dist:
			best_col_index = i
			best_dist = cur_dist

	return best_col_index

def int_to_unary_code(value: int) -> tuple[int, int]:
	if value == 0:
		raise ValueError("Cannot convert 0 to unary.")
	elif value < 0:
		raise ValueError("Cannot convert negative values to unary.")

	return ((1 << value) - 2, value)

def int_to_pokecode(value: int) -> tuple[int, int]:
	if value == 0:
		raise ValueError("Cannot convert 0 to pokecode.")
	elif value < 0:
		raise ValueError("Cannot convert negative values to pokecode.")

	value += 1
	mag = math.floor(math.log2(value))

	return ((int_to_unary_code(mag)[0] << mag) | (value & ((1 << mag) - 1)), mag*2)

def deltacode(bits: Bits) -> Bits:
	output = Bits()

	prev = False
	for i in range(len(bits)):
		if bits[i] != prev:
			output.push(1, 1)
		else:
			output.push(0, 1)
		prev = bits[i]

	return output

def runlength_encode(bits: Bits) -> Bits:
	bits_pos = bits.tell()
	bits.seek(0)

	output = Bits()

	packet_type = 0 if bits.read(2)[0] == 0 else 1
	output.push(packet_type, 1)
	bits.seek(0)

	run_length = 0
	for i in range(0, len(bits), 2):
		data = bits.read(2)[0]

		if data == 0:
			if packet_type:
				output.push(0, 2)
				run_length = 0
			
			run_length += 1
		
		else:
			if not packet_type:
				output.push(int_to_pokecode(run_length))

			output.push(data, 2)

		packet_type = 0 if data == 0 else 1

	if packet_type == 0:
		output.push(int_to_pokecode(run_length))

	bits.seek(bits_pos)
	return output

def compress_image(image: Image.Image) -> bytes:
	img_width = (image.width + 7) // 8
	img_height = (image.height + 7) // 8

	print(f"Image size: {img_width}x{img_height}")

	if max(img_width, img_height) > 16:
		raise OverflowError(f"Cannot encode images larger than 128x128 (image is {img_width*8}x{img_height*8}).")

	output = Bits()

	output.push(img_width, 4)
	output.push(img_height, 4)

	bitplane = [Bits(), Bits()]

	for y in range(img_height*8):
		for x in range(img_width*8):
			col = get_closest_color_from_palette(image_get_pixel(image, x, y))

			bitplane[0].push(col & 1, 1)
			bitplane[1].push(col >> 1, 1)


	compressed_sprite_data = []

	for flip in range(2):
		#cur_bitplane = (bitplane[1], bitplane[0]) if flip else (bitplane[0], bitplane[1])
		for mode in range(3):
			buffer = Bits()

			bitplane_a = Bits()
			bitplane_b = Bits()
			if not flip:
				bitplane_a.push(bitplane[0])
				bitplane_b.push(bitplane[1])
			else:
				bitplane_a.push(bitplane[1])
				bitplane_b.push(bitplane[0])

			buffer.push(flip, 1)

			compressed_bitplane = (None, None)
			match mode:
				case 0:
					compressed_bitplane = (
						deltacode(bitplane_a),
						deltacode(bitplane_b)
					)
				case 1:
					compressed_bitplane = (
						deltacode(bitplane_a),
						bitplane_a ^ bitplane_b
					)
				case 2:
					compressed_bitplane = (
						deltacode(bitplane_a),
						deltacode(bitplane_a ^ bitplane_b)
					)
			
			compressed_bitplane = (
				runlength_encode(compressed_bitplane[0]),
				runlength_encode(compressed_bitplane[1])
			)
			
			buffer.push(compressed_bitplane[0])
			buffer.push([(0,1),(2,2),(3,2)][mode])
			buffer.push(compressed_bitplane[1])
			
			print(f"{'flip' if flip else '----'} mode {mode} : {len(buffer)}")
			compressed_sprite_data.append(buffer)

	best_index = -1
	best_length = 1e100
	for i in range(len(compressed_sprite_data)):
		if len(compressed_sprite_data[i]) < best_length:
			best_index = i
			best_length = len(compressed_sprite_data[i])

	return compressed_sprite_data[best_index].to_bytes()

if __name__ == "__main__":

	if len(sys.argv) != 3:
		print(f"Usage: {sys.argv[0]} image output")
		exit(1)
	
	in_image = Image.open(sys.argv[1])
	compressed = compress_image(in_image)

	out_file = open(sys.argv[2], "wb")
	out_file.write(compressed)
