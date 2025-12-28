fun max(x, y) = if x > y then x else y

fun kadane nil _ glob = glob
  | kadane (h::t) curr glob =
    let
      val next_curr = max(h, curr + h)
      val next_glob = max(glob, next_curr)
    in
      kadane t next_curr next_glob
    end

fun find_max_subarray (h::t) = kadane t h h

val result = find_max_subarray [~2, 1, ~3, 4, ~1, 2, 1, ~5, 4]
