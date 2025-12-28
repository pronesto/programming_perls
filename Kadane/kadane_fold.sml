fun max(x, y) = if x > y then x else y

fun next_sub_array(x, (curr, glob)) =
    let
      val next_curr = max(x, curr + x)
      val next_glob = max(glob, next_curr)
    in
      (next_curr, next_glob)
    end

fun find_max_subarray (h::t) = #2 (foldl next_sub_array (h, h) t)

val result = find_max_subarray [~2, 1, ~3, 4, ~1, 2, 1, ~5, 4]
