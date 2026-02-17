CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
TARGET = a.out
OBJDIR = objects

SOURCES = main.cpp Account/Account.cpp Bank/Bank.cpp
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re


