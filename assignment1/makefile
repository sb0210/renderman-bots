SRCS=$(wildcard shaders/*.sl)
OBJS=$(SRCS:.sl=.slo)

shaders/%.slo: shaders/%.sl
	cd shaders && RMANTREE=/opt/pixar/RenderManProServer-20.6/ shader ../$<


all: droid
	
.PHONY: shaders
shaders: $(OBJS)

.PHONY: droid
droid: shaders
	prman droids.rib

