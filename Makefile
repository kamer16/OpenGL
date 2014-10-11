-include Makefile.sources

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Wconversion
CPPFLAGS =
LDFLAGS = -lGL -lglfw -lGLEW -lGLU -lSOIL

TARGET=main

all:$(TARGET)

run:$(TARGET)
	./$(TARGET)

$(TARGET):$(OBJS)
	$(CXX)  $(LDFLAGS) $(OBJS_DIR) -o $(TARGET)

%.o: src/%.cpp
	@ mkdir -p _build
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o _build/$@
	@ $(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -o _build/$*.d

clean:
	$(RM) -r _build $(TARGET)

-include $(patsubst %.cpp, _build/%.d, $(SOURCES))