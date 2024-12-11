defmodule StoneEvolution do
  def main(blinks) do
    input = File.read!("input.txt")
    |> String.trim()
    |> String.split()
    |> Enum.map(&String.to_integer/1)

    if :ets.info(:cache) == :undefined do
      :ets.new(:cache, [:named_table, :public, :set, read_concurrency: true])
    end

    total =
      Enum.map(input, &count_stones(&1, blinks))
      |> Enum.sum()

    IO.puts("Total stones after #{blinks} blinks: #{total}")
  end

  def count_stones(_, 0), do: 1

  def count_stones(num, blinks) do
    case :ets.lookup(:cache, {num, blinks}) do
      [{_, res}] ->
        res

      [] ->
        next = transform(num)
        res = Enum.map(next, &count_stones(&1, blinks - 1)) |> Enum.sum()
        :ets.insert(:cache, {{num, blinks}, res})
        res
    end
  end

  def transform(0), do: [1]

  def transform(n) do
    str = Integer.to_string(n)
    len = String.length(str)

    if rem(len, 2) == 0 do
      mid = div(len, 2)
      left = String.slice(str, 0, mid) |> to_int()
      right = String.slice(str, mid, len) |> to_int()
      [left, right]
    else
      [n * 2024]
    end
  end

  defp to_int(""), do: 0
  defp to_int(s), do: String.to_integer(s)
end

StoneEvolution.main(25)
StoneEvolution.main(75)

