CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -lsqlite3
SRC = src/main.c src/ticket.c src/database.c src/login.c src/dashboard.c src/ticket_form.c
OBJ_DIR = obj
OBJS = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

todo_app: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I./include -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) todo_app

.PHONY: clean
