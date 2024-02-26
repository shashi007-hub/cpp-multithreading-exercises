CXX ?= c++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pthread -I.
BUILD := build
EXERCISE = $(firstword $(wildcard exercises/$(1)-*))
SOLUTIONS := $(wildcard exercises/*/solution.cpp)

.PHONY: all-solutions clean

all-solutions:
	@for source in $(SOLUTIONS); do \
		echo "== $$source"; \
		mkdir -p $(BUILD) && $(CXX) $(CXXFLAGS) $$source -o $(BUILD)/solution && $(BUILD)/solution || exit 1; \
	done

test-%:
	@mkdir -p $(BUILD)
	@$(CXX) $(CXXFLAGS) -Wno-unused-parameter -Wno-unused-variable $(call EXERCISE,$*)/starter.cpp -o $(BUILD)/starter-$*
	@$(BUILD)/starter-$*

solution-%:
	@mkdir -p $(BUILD)
	@$(CXX) $(CXXFLAGS) $(call EXERCISE,$*)/solution.cpp -o $(BUILD)/solution-$*
	@$(BUILD)/solution-$*

clean:
	rm -rf $(BUILD)
