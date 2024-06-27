# Scandal

 - Complexity:
   - Time: O(n + m)
      - Explanation: Tarjan -> O(n + m). Iterating through all SCCs and for each SCC we iterate through each node so we iterate through each node of the graph -> O(n). O(n + m + n) = O(n + m).
   - Space: O(n + m)
      - Explanation: Tarjan -> O(2 * n) = O(n). Graph is represented as an adjacency graph with double the nodes -> O(2 * n + m) = O(n + m). O(n + n + m) = O(n + m).
 - We read the friends and the rules. Depending on the rule we add 2 edges in the graph using the logic of the affirmations:
    - c = 0: Either x or y must join the party <=> x or y <=> if x doesn't go, y must go and if y doesn't go, x must go <=> !x -> y and !y -> x.
    - c = 1: If x doesn't go, y mustn't go <=> !x -> !y <=> y -> x. We take both cases.
    - c = 2: If y doesn't go, x mustn't go <=> !y -> !x <=> x -> y. We take both cases.
    - c = 3: Either x or y mustn't join the party <=> !x or !y <=> if x goes, y must not go and if y goes, x must not go <=> x -> !y and y -> !x.
 - This way the resulting graph will contain multiple strongly connected components (SCCs) that represent the restrictions between friends.
 - We use Tarjan to find these SCCs and check for each if there are any contradictions (a friend both must and mustn't be invited to the party).
 - If all rules are respected, we print the number of friends to invite and each of those friends.
