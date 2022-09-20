import pog

def pre_loop():
	print("This is called before the loop")

def post_loop():
	print("This is called after the loop")

pog.init()
c = pog.Context(title="Hello, World!", width=800, height=600)
c.add_handler("preloop", pre_loop)
c.add_handler("postloop", post_loop)
c.run()
pog.quit()
