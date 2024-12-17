const std = @import("std");

const Registers = struct {
    a: i64 = 0,
    b: i64 = 0,
    c: i64 = 0,
};

fn adv(regs: *Registers, operand: u8) void {
    const divisor = std.math.pow(i64, 2, getComboValue(regs.*, operand));
    regs.a = @divTrunc(regs.a, divisor);
}

fn bxl(regs: *Registers, operand: u8) void {
    regs.b = regs.b ^ operand;
}

fn bst(regs: *Registers, operand: u8) void {
    regs.b = @intCast(@mod(getComboValue(regs.*, operand), 8));
}

fn jnz(regs: Registers, _: u8) bool {
    return regs.a != 0;
}

fn bxc(regs: *Registers, _: u8) void {
    regs.b = regs.b ^ regs.c;
}

fn out(regs: Registers, operand: u8) u8 {
    return @intCast(@mod(getComboValue(regs, operand), 8));
}

fn bdv(regs: *Registers, operand: u8) void {
    const divisor = std.math.pow(i64, 2, getComboValue(regs.*, operand));
    regs.b = @divTrunc(regs.a, divisor);
}

fn cdv(regs: *Registers, operand: u8) void {
    const divisor = std.math.pow(i64, 2, getComboValue(regs.*, operand));
    regs.c = @divTrunc(regs.a, divisor);
}

fn getComboValue(regs: Registers, operand: u8) i64 {
    return switch (operand) {
        0...3 => operand,
        4 => regs.a,
        5 => regs.b,
        6 => regs.c,
        else => unreachable,
    };
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const file = try std.fs.cwd().openFile("input.txt", .{});
    defer file.close();
    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();
    var buf: [1024]u8 = undefined;

    var regs = Registers{};
    _ = try in_stream.readUntilDelimiter(&buf, ':');
    var line = try in_stream.readUntilDelimiter(&buf, '\n');
    regs.a = try std.fmt.parseInt(i64, std.mem.trim(u8, line, " "), 10);

    _ = try in_stream.readUntilDelimiter(&buf, ':');
    line = try in_stream.readUntilDelimiter(&buf, '\n');
    regs.b = try std.fmt.parseInt(i64, std.mem.trim(u8, line, " "), 10);

    _ = try in_stream.readUntilDelimiter(&buf, ':');
    line = try in_stream.readUntilDelimiter(&buf, '\n');
    regs.c = try std.fmt.parseInt(i64, std.mem.trim(u8, line, " "), 10);

    _ = try in_stream.readUntilDelimiter(&buf, '\n');
    _ = try in_stream.readUntilDelimiter(&buf, ':');
    line = try in_stream.readUntilDelimiter(&buf, '\n');

    var program = std.ArrayList(u8).init(allocator);
    defer program.deinit();
    var it = std.mem.tokenize(u8, std.mem.trim(u8, line, " "), ",");
    while (it.next()) |num_str| {
        const num = try std.fmt.parseInt(u8, num_str, 10);
        try program.append(num);
    }

    var outputs = std.ArrayList(u8).init(allocator);
    defer outputs.deinit();
    var ip: usize = 0;
    while (ip < program.items.len - 1) {
        const opcode = program.items[ip];
        const operand = program.items[ip + 1];
        switch (opcode) {
            0 => adv(&regs, operand),
            1 => bxl(&regs, operand),
            2 => bst(&regs, operand),
            3 => {
                if (jnz(regs, 0)) {
                    ip = operand;
                    continue;
                }
            },
            4 => bxc(&regs, operand),
            5 => try outputs.append(out(regs, operand)),
            6 => bdv(&regs, operand),
            7 => cdv(&regs, operand),
            else => unreachable,
        }
        ip += 2;
    }

    const stdout = std.io.getStdOut().writer();
    for (outputs.items, 0..) |val, i| {
        if (i > 0) try stdout.print(",", .{});
        try stdout.print("{d}", .{val});
    }
    try stdout.print("\n", .{});
}

