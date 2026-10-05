import subprocess
from PIL import Image

# you must run this in the same directory as the script!
# and also you must have rsvg-convert
# on ubuntu you can get this with `apt install librsvg2-bin`

outlines = [
"outline_hexagon",
"outline_circle",
"outline_square",
"outline_octagon",
]

sprites = [
"fill_hexagon",
"fill_circle",
"fill_square",
"fill_octagon",
]

# the green gradient colors we are replacing in the base svgs
color_from = ["#D6FF5A", "#66CC11", "#90FF44", "#449906"]

# each pair of colors represents a gradient,
# first pair is for the top part of the button,
# second pair is for the bottom.
colors = {
    "Green": ["#D6FF5A", "#66CC11", "#90FF44", "#449906"],
    "Pink": ["#FACCFC", "#FF71FF", "#FBB1FF", "#FB58FF"],
    "Cyan": ["#58FDFA", "#13D5EA", "#44F9F6", "#0B9FBE"],
    "Blue": ["#1AF1F8", "#0AB4FF", "#23DCFA", "#0077FA"],
    "Gray": ["#DEDEE0", "#979997", "#CACCCA", "#747472"],
    "DarkPurple": ["#41384b", "#2f2937", "#393142", "#221e28"],
    "DarkAqua": ["#2a4559", "#1f3441", "#253d4e", "#17272f"],
    "Red": ["#FF5A5A", "#CC1111", "#FF4444", "#990606"],
    "FLAG": ["#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF"],
}

def hex_to_rgb(hex_str):
    hex_str = hex_str.lstrip('#')
    return tuple(int(hex_str[i:i+2], 16) for i in (0, 2, 4))

flags = {
    "Lesbian": [ {"colour": "#D62800", "val": 0.2}, {"colour": "#FF9B56", "val": 0.4}, {"colour": "#FFFFFF", "val": 0.6}, {"colour": "#D462A6", "val": 0.8}, {"colour": "#A40062", "val": 1.0} ],
    "Trans": [ {"colour": "#5BCEFA", "val": 0.2}, {"colour": "#F5A9B8", "val": 0.4}, {"colour": "#FFFFFF", "val": 0.6}, {"colour": "#F5A9B8", "val": 0.8}, {"colour": "#5BCEFA", "val": 1.0} ],
    "Gay": [ {"colour": "#068d70", "val": 0.2}, {"colour": "#98e8c1", "val": 0.4}, {"colour": "#FFFFFF", "val": 0.6}, {"colour": "#7bade2", "val": 0.8}, {"colour": "#3d1a78", "val": 1.0} ],
    "Pride": [ {"colour": "#e50000", "val": (1/6)*1}, {"colour": "#ff8d00", "val": (1/6)*2}, {"colour": "#ffee00", "val": (1/6)*3}, {"colour": "#028121", "val": (1/6)*4}, {"colour": "#004cff", "val": (1/6)*5}, {"colour": "#770088", "val": (1/6)*6} ],
    "Bisexual": [ {"colour": "#D60270", "val": 0.4}, {"colour": "#9b4f96", "val": 0.6}, {"colour": "#0038a8", "val": 1.0} ],
    "Pansexual": [ {"colour": "#FF218C", "val": (1/3)*1}, {"colour": "#FFD800", "val": (1/3)*2}, {"colour": "#21B1FF", "val": 1.0} ],
}

for outline in outlines:
	with open(f"svgs/{outline}.svg", "r") as file:
		svg_base = file.read()
		out = f"out/shortcut-{outline}.png"
		subprocess.run(["rsvg-convert", "-o", out], input=svg_base.encode())

for sprite in sprites:
    with open(f"svgs/{sprite}.svg", "r") as file:
        svg_base = file.read()
    for name, cols in colors.items():
        svg = svg_base
        out = f"out/shortcut-{sprite}_{name}.png"
        print(f"Generating {out}")
        for color_orig, color_to in zip(color_from, cols):
            svg = svg.replace(color_orig, color_to)
        subprocess.run(["rsvg-convert", "-o", out], input=svg.encode())

    base_img = Image.open(f"out/shortcut-{sprite}_FLAG.png").convert("RGBA")
    bbox = base_img.getbbox()

    y_min = bbox[1]
    y_max = bbox[3]
    y_range = y_max - y_min

    for name, cols in flags.items():
        img = base_img.copy()
        pixels = img.load()

        for y in range(img.height):
            for x in range(img.width):
                r, g, b, a = pixels[x, y]
                if a > 0:
                    if y_range > 0:
                        normalized_y = (y - y_min) / y_range
                    else:
                        normalized_y = 0.0

                    normalized_y = max(0.0, min(1.0, normalized_y))
                    for item in cols:
                        if normalized_y <= item["val"]:
                            target_rgb = hex_to_rgb(item["colour"])
                            r = int((target_rgb[0] / 255.0) * r)
                            g = int((target_rgb[1] / 255.0) * g)
                            b = int((target_rgb[2] / 255.0) * b)
                            break

                    pixels[x, y] = (r, g, b, a)

        img.save(f"out/shortcut-{sprite}_{name}.png")
