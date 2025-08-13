#include "../uart/uart0.h"
#include "../mailbox/mbox.h"
#include "../kernel/utils.h"
#include "./cli.h"
#include "../kernel/types.h"

#define PROMPT "MyOS> "
#define MAX_LINE 128
#define MAX_ARGS 8
#define HIST_SIZE 8

typedef int (*cmd_fn)(int argc, char **argv);

typedef struct
{
    const char *name;
    const char *help;
    cmd_fn fn;
} command_t;

/* Forward decls */
static int cmd_help(int argc, char **argv);
static int cmd_clear(int argc, char **argv);
static int cmd_showinfo(int argc, char **argv);
static int cmd_baudrate(int argc, char **argv);
static int cmd_handshake(int argc, char **argv);
static int cmd_task2(int argc, char **argv);
static int cmd_task3(int argc, char **argv);

static command_t commands[] = {
    {"help", "help [cmd] - Show help", cmd_help},
    {"clear", "clear - Clear screen", cmd_clear},
    {"showinfo", "showinfo - Show board revision and MAC", cmd_showinfo},
    {"baudrate", "baudrate <9600|19200|38400|57600|115200> - Set baud", cmd_baudrate},
    {"handshake", "handshake <on|off> - Enable/disable CTS/RTS", cmd_handshake},
    {"task2", "task2 - Run Task 2 demo (placeholder)", cmd_task2},
    {"task3", "task3 - Run Task 3 demo (placeholder)", cmd_task3},
};
static const int NCMDS = sizeof(commands) / sizeof(commands[0]);

/* History */
static char history[HIST_SIZE][MAX_LINE];
static int hist_count = 0;
static int hist_index = 0;

static void print_prompt(void) { uart_puts(PROMPT); }

static void put_hex32(uint32_t v)
{
    const char *hex = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4)
    {
        uart_sendc(hex[(v >> i) & 0xF]);
    }
}

static void put_mac(uint8_t mac[6])
{
    const char *hex = "0123456789ABCDEF";
    for (int i = 0; i < 6; i++)
    {
        uart_sendc(hex[(mac[i] >> 4) & 0xF]);
        uart_sendc(hex[mac[i] & 0xF]);
        if (i < 5)
            uart_sendc(':');
    }
}

static command_t *find_command(const char *name)
{
    for (int i = 0; i < NCMDS; i++)
        if (u_strcmp(name, commands[i].name) == 0)
            return &commands[i];
    return 0;
}

static int tokenize(char *line, char **argv)
{
    int argc = 0;
    char *p = line;
    while (*p && argc < MAX_ARGS)
    {
        while (*p && u_isspace(*p))
            *p++ = 0;
        if (!*p)
            break;
        argv[argc++] = p;
        while (*p && !u_isspace(*p))
            p++;
    }
    return argc;
}

static void add_history(const char *line)
{
    if (!line || !*line)
        return;
    int slot = hist_count % HIST_SIZE;
    int i = 0;
    while (line[i] && i < MAX_LINE - 1)
    {
        history[slot][i] = line[i];
        i++;
    }
    history[slot][i] = 0;
    hist_count++;
    hist_index = hist_count;
}

static int history_get_prev(char *buf)
{
    if (hist_count == 0)
        return 0;
    if (hist_index == 0)
        hist_index = 0;
    else
        hist_index--;
    int idx = (hist_index % HIST_SIZE + HIST_SIZE) % HIST_SIZE;
    int i = 0;
    while (history[idx][i])
    {
        buf[i] = history[idx][i];
        i++;
        if (i >= MAX_LINE - 1)
            break;
    }
    buf[i] = 0;
    return 1;
}

static int history_get_next(char *buf)
{
    if (hist_count == 0)
        return 0;
    if (hist_index < hist_count)
        hist_index++;
    if (hist_index == hist_count)
    {
        buf[0] = 0;
        return 1;
    }
    int idx = (hist_index % HIST_SIZE + HIST_SIZE) % HIST_SIZE;
    int i = 0;
    while (history[idx][i])
    {
        buf[i] = history[idx][i];
        i++;
        if (i >= MAX_LINE - 1)
            break;
    }
    buf[i] = 0;
    return 1;
}

static void redraw_line(const char *buf)
{
    uart_puts("\r\n");
    print_prompt();
    uart_puts(buf);
}

/* Tab completion: if one match, complete; if many, list */
static void complete(char *buf, int *len)
{
    int matches[16];
    int m = 0;
    for (int i = 0; i < NCMDS; i++)
    {
        if (u_strncmp(commands[i].name, buf, (size_t)*len) == 0)
        {
            if (m < 16)
                matches[m++] = i;
        }
    }
    if (m == 0)
        return;
    if (m == 1)
    {
        /* complete */
        const char *name = commands[matches[0]].name;
        int n = (int)u_strlen(name);
        for (int i = *len; i < n && i < MAX_LINE - 1; i++)
        {
            buf[i] = name[i];
        }
        for (int i = *len; i < n; i++)
        {
            uart_sendc(name[i]);
        }
        *len = n;
        buf[*len] = 0;
    }
    else
    {
        uart_puts("\r\n");
        for (int i = 0; i < m; i++)
        {
            uart_puts(commands[matches[i]].name);
            uart_puts("  ");
        }
        redraw_line(buf);
    }
}

/* Built-in command implementations */
static int cmd_help(int argc, char **argv)
{
    if (argc == 2)
    {
        command_t *c = find_command(argv[1]);
        if (!c)
        {
            uart_puts("Unknown command\n");
            return 0;
        }
        uart_puts(c->help);
        uart_puts("\n");
        return 0;
    }
    uart_puts("Commands:\n");
    for (int i = 0; i < NCMDS; i++)
    {
        uart_puts("  ");
        uart_puts(commands[i].help);
        uart_puts("\n");
    }
    return 0;
}

static int cmd_clear(int argc, char **argv)
{
    /* ANSI clear screen + home; if terminal doesn't support, it's harmless */
    uart_puts("\033[2J\033[H");
    return 0;
}

static int cmd_showinfo(int argc, char **argv)
{
    uint32_t rev = 0;
    uint8_t mac[6] = {0};
    int ok1 = mbox_get_board_revision(&rev);
    int ok2 = mbox_get_mac(mac);
    uart_puts("Board revision: 0x");
    put_hex32(rev);
    if (ok1 != 0)
        uart_puts(" (unavailable in QEMU)");
    uart_puts("\nMAC: ");
    if (ok2 == 0)
    {
        put_mac(mac);
    }
    else
    {
        uart_puts("unavailable in QEMU");
    }
    uart_puts("\n");
    return 0;
}

static int cmd_baudrate(int argc, char **argv)
{
    if (argc != 2)
    {
        uart_puts("Usage: baudrate <9600|19200|38400|57600|115200>\n");
        return 0;
    }
    const char *s = argv[1];
    unsigned int b = 0;
    for (int i = 0; s[i]; i++)
    {
        if (!u_isdigit(s[i]))
        {
            uart_puts("Invalid baud\n");
            return 0;
        }
        b = b * 10 + (unsigned)(s[i] - '0');
    }
    if (uart_set_baud(b) == 0)
    {
        uart_puts("OK. Please change your terminal to ");
        uart_puts(argv[1]);
        uart_puts(" and hit Enter.\n");
    }
    else
    {
        uart_puts("Unsupported baud.\n");
    }
    return 0;
}

static int cmd_handshake(int argc, char **argv)
{
    if (argc != 2)
    {
        uart_puts("Usage: handshake <on|off>\n");
        return 0;
    }
    if (u_strcmp(argv[1], "on") == 0)
    {
        uart_set_flow(1);
        uart_puts("CTS/RTS enabled\n");
    }
    else if (u_strcmp(argv[1], "off") == 0)
    {
        uart_set_flow(0);
        uart_puts("CTS/RTS disabled\n");
    }
    else
        uart_puts("Usage: handshake <on|off>\n");
    return 0;
}

static int cmd_task2(int argc, char **argv)
{
    uart_puts("Task 2 placeholder. Use this later to launch image/video demo.\n");
    return 0;
}
static int cmd_task3(int argc, char **argv)
{
    uart_puts("Task 3 placeholder. Use this later to launch the game.\n");
    return 0;
}

static void execute(char *line)
{
    char *argv[MAX_ARGS] = {0};
    int argc = tokenize(line, argv);
    if (argc == 0)
        return;
    command_t *cmd = find_command(argv[0]);
    if (!cmd)
    {
        uart_puts("Unknown command. Type 'help'.\n");
        return;
    }
    cmd->fn(argc, argv);
}

void cli_run(void)
{
    char buf[MAX_LINE] = {0};
    int len = 0;
    print_prompt();
    for (;;)
    {
        char c = uart_getc();
        if (c == '\n')
        {
            uart_puts("\n");
            buf[len] = 0;
            if (len > 0)
                add_history(buf);
            execute(buf);
            len = 0;
            buf[0] = 0;
            print_prompt();
        }
        else if (c == '\t')
        {
            complete(buf, &len);
        }
        else if (c == 0x7F || c == '\b')
        {
            if (len > 0)
            {
                len--;
                uart_puts("\b \b");
            }
        }
        else if (c == '_')
        { /* history up */
            if (history_get_prev(buf))
            {
                len = (int)u_strlen(buf);
                redraw_line(buf);
            }
        }
        else if (c == '+')
        { /* history down */
            if (history_get_next(buf))
            {
                len = (int)u_strlen(buf);
                redraw_line(buf);
            }
        }
        else
        {
            if (len < MAX_LINE - 1)
            {
                buf[len++] = c;
                uart_sendc(c);
            }
        }
    }
}
