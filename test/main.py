import pog

def pre_loop():
	print("This is called before the loop")

def post_loop():
	print("This is called after the loop")

def render_func():
    pog.draw_rectangle((20, 20), (150, 150, 50), (100, 200))

pog.init()
c = pog.Context(title="Hello, World!", width=800, height=600)
c.add_handler("preloop", pre_loop)
c.add_handler("render", render_func)
c.add_handler("postloop", post_loop)
c.run()
pog.quit()
