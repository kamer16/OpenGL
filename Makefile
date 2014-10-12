-include Makefile.sources

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Wconversion -g
CPPFLAGS =
LDFLAGS = -lGL -lglfw -lGLEW -lSOIL -lGLU

TARGET=main

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX)  $(LDFLAGS) $(OBJS) -o $(TARGET)

run:$(TARGET)
	./$(TARGET)

_build/%.o: src/%.cpp
	@ mkdir -p _build
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
	@ $(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -o _build/$*.d
	@mv -f _build/$*.d _build/$*.d.tmp
	@sed -e 's|.*:|_build/$*.o:|' < _build/$*.d.tmp > _build/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < _build/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> _build/$*.d
	@rm -f _build/$*.d.tmp

clean:
	$(RM) -r _build $(TARGET)

-include $(OBJS:.o=.d)
