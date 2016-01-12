ERLC:=$(shell which erlc)
ESCRIPT=$(shell which escript)
ERL=erl -boot start_clean
ifndef ${START}
	START:=start
endif

.SUFFIXES: .erl .beam .yrl

.erl.beam:
	$(ERLC) +debug_info -W $<

.yrl.erl:
	$(ERLC) -W $<

# Modules that need compiling e.g.
# MODS=p01

# add subdirs???
compile:${MODS:%=%.beam}

erlclean:
	\rm -f *.beam *~
	

## special compilation requirements are added here

## run an application from the makefile
#application1:compile
#	${ERL}

run:compile
	erl -noshell -s ${MOD} ${START} -s init stop
