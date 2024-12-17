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

fn runProgram(initial_a: i64, program: []const u8, allocator: std.mem.Allocator) ![]u8 {
    var regs = Registers{ .a = initial_a };
    var outputs = std.ArrayList(u8).init(allocator);
    errdefer outputs.deinit();
    var ip: usize = 0;
    while (ip < program.len - 1) {
        const opcode = program[ip];
        const operand = program[ip + 1];
        switch (opcode) {
            0 => adv(&regs, operand),
            1 => bxl(&regs, operand),
            2 => bst(&regs, operand),
            3 => if (regs.a != 0) {
                ip = operand;
                continue;
            },
            4 => bxc(&regs, operand),
            5 => try outputs.append(out(regs, operand)),
            6 => bdv(&regs, operand),
            7 => cdv(&regs, operand),
            else => unreachable,
        }
        ip += 2;
    }
    return outputs.toOwnedSlice();
}

fn checkOutputSuffix(program: []const u8, output: []const u8, depth: usize) bool {
    if (output.len < depth) return false;
    const program_suffix = program[program.len - depth..];
    const output_suffix = output[output.len - depth..];
    for (program_suffix, output_suffix) |p, o| {
        if (p != o) return false;
    }
    return true;
}

fn findSolution(program: []const u8, current_sol: i64, depth: usize, allocator: std.mem.Allocator) !?i64 {
    if (depth > program.len) return current_sol;
    var i: i64 = 0;
    while (i < 8) : (i += 1) {
        const test_value = (current_sol << 3) + i;
        const output = try runProgram(test_value, program, allocator);
        defer allocator.free(output);
        if (checkOutputSuffix(program, output, depth)) {
            if (try findSolution(program, test_value, depth + 1, allocator)) |solution| {
                return solution;
            }
        }
    }
    return null;
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

    _ = try in_stream.readUntilDelimiter(&buf, '\n');
    _ = try in_stream.readUntilDelimiter(&buf, '\n');
    _ = try in_stream.readUntilDelimiter(&buf, '\n');
    _ = try in_stream.readUntilDelimiter(&buf, '\n');

    _ = try in_stream.readUntilDelimiter(&buf, ':');
    const line = try in_stream.readUntilDelimiter(&buf, '\n');

    var program = std.ArrayList(u8).init(allocator);
    defer program.deinit();

    var it = std.mem.tokenize(u8, std.mem.trim(u8, line, " "), ",");
    while (it.next()) |num_str| {
        const num = try std.fmt.parseInt(u8, num_str, 10);
        try program.append(num);
    }

    if (try findSolution(program.items, 0, 1, allocator)) |solution| {
        const stdout = std.io.getStdOut().writer();
        try stdout.print("{d}\n", .{solution});
    }
}

