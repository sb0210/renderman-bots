SRCS=$(wildcard shaders/*.sl)
OBJS=$(SRCS:.sl=.slo)

shaders/%.slo: shaders/%.sl
	cd shaders && shader ../$<

droid: $(OBJS)
	prman droids.rib

all: droid
