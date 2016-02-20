SRCS=$(wildcard shaders/*.sl)
OBJS=$(SRCS:.sl=.slo)

CAMERAS=$(wildcard cameras/*.rib)
TIFFS=$(CAMERAS:.rib=.tif)

shaders/%.slo: shaders/%.sl
	cd shaders && RMANTREE=/opt/pixar/RenderManProServer-20.6/ shader ../$<

cameras/%.tif: cameras/%.rib
	prman $<


all: droid camera
	
.PHONY: shaders
shaders: $(OBJS)

.PHONY: camera
camera: $(TIFFS)

.PHONY: droid
droid: shaders
	prman droids.rib

