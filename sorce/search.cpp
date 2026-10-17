#include "../include/search.h"

#include <cctype>
#include <cstdlib>
#include <cstring>

/* ---------- C-style queue (circular array) for BFS ---------- */
struct Queue {
  int data[SEARCH_MAX_NODES];
  int front;
  int rear;
  int size;
};

static void queueInit(Queue *q) {
  q->front = 0;
  q->rear = -1;
  q->size = 0;
}

static int queueIsEmpty(const Queue *q) { return q->size == 0; }

static int queueIsFull(const Queue *q) { return q->size == SEARCH_MAX_NODES; }

static void enqueue(Queue *q, int value) {
  if (queueIsFull(q))
    return;
  q->rear = (q->rear + 1) % SEARCH_MAX_NODES;
  q->data[q->rear] = value;
  q->size++;
}

static int dequeue(Queue *q) {
  if (queueIsEmpty(q))
    return -1;
  int value = q->data[q->front];
  q->front = (q->front + 1) % SEARCH_MAX_NODES;
  q->size--;
  return value;
}

/* ---------- C-style stack (array) for DFS ---------- */
struct Stack {
  int data[SEARCH_MAX_NODES];
  int depth[SEARCH_MAX_NODES];
  int top;
};

static void stackInit(Stack *s) { s->top = -1; }

static int stackIsEmpty(const Stack *s) { return s->top < 0; }

static int stackIsFull(const Stack *s) {
  return s->top >= SEARCH_MAX_NODES - 1;
}

static void stackPush(Stack *s, int value, int depth) {
  if (stackIsFull(s))
    return;
  s->top++;
  s->data[s->top] = value;
  s->depth[s->top] = depth;
}

static int stackPop(Stack *s, int *depthOut) {
  if (stackIsEmpty(s))
    return -1;
  if (depthOut)
    *depthOut = s->depth[s->top];
  return s->data[s->top--];
}

static int validNodeId(int id) { return id >= 0 && id < SEARCH_MAX_NODES; }

static IntList makeIntList(int capacity) {
  IntList list;
  list.count = 0;
  list.items = nullptr;
  if (capacity <= 0)
    return list;
  list.items = (int *)std::malloc((size_t)capacity * sizeof(int));
  return list;
}

static void appendInt(IntList *list, int value, int capacity) {
  if (!list || !list->items || list->count >= capacity)
    return;
  list->items[list->count++] = value;
}

void freeIntList(IntList *list) {
  if (!list)
    return;
  if (list->items) {
    std::free(list->items);
    list->items = nullptr;
  }
  list->count = 0;
}

static TraversalResult makeTraversalResult(int capacity) {
  TraversalResult result;
  result.count = 0;
  result.items = nullptr;
  result.hops = nullptr;
  if (capacity <= 0)
    return result;
  result.items = (int *)std::malloc((size_t)capacity * sizeof(int));
  result.hops = (int *)std::malloc((size_t)capacity * sizeof(int));
  if (!result.items || !result.hops) {
    std::free(result.items);
    std::free(result.hops);
    result.items = nullptr;
    result.hops = nullptr;
  }
  return result;
}

static void appendVisit(TraversalResult *result, int nodeId, int hops,
                        int capacity) {
  if (!result || !result->items || !result->hops || result->count >= capacity)
    return;
  result->items[result->count] = nodeId;
  result->hops[result->count] = hops;
  result->count++;
}

void freeTraversalResult(TraversalResult *result) {
  if (!result)
    return;
  if (result->items) {
    std::free(result->items);
    result->items = nullptr;
  }
  if (result->hops) {
    std::free(result->hops);
    result->hops = nullptr;
  }
  result->count = 0;
}

static void toLowerCopy(const char *src, char *dst, int dstSize) {
  int i = 0;
  if (!src || dstSize <= 0) {
    if (dstSize > 0)
      dst[0] = '\0';
    return;
  }
  while (src[i] != '\0' && i < dstSize - 1) {
    dst[i] = (char)std::tolower((unsigned char)src[i]);
    i++;
  }
  dst[i] = '\0';
}

/* ---------- BFS / DFS ---------- */
TraversalResult bfsTraversal(const Graph &graph, int startNode) {
  TraversalResult order = makeTraversalResult(SEARCH_MAX_NODES);
  if (!validNodeId(startNode) || !order.items)
    return order;

  int visited[SEARCH_MAX_NODES];
  int dist[SEARCH_MAX_NODES];
  for (int i = 0; i < SEARCH_MAX_NODES; i++) {
    visited[i] = 0;
    dist[i] = 0;
  }

  Queue q;
  queueInit(&q);
  visited[startNode] = 1;
  dist[startNode] = 0;
  enqueue(&q, startNode);

  while (!queueIsEmpty(&q)) {
    int u = dequeue(&q);
    appendVisit(&order, u, dist[u], SEARCH_MAX_NODES);

    EdgeNode *edge = graph.getNeighbors(u);
    while (edge != nullptr) {
      int v = edge->to_node;
      if (validNodeId(v) && !visited[v]) {
        visited[v] = 1;
        dist[v] = dist[u] + 1;
        enqueue(&q, v);
      }
      edge = edge->next;
    }
  }

  return order;
}

TraversalResult dfsTraversal(const Graph &graph, int startNode) {
  TraversalResult order = makeTraversalResult(SEARCH_MAX_NODES);
  if (!validNodeId(startNode) || !order.items)
    return order;

  int visited[SEARCH_MAX_NODES];
  for (int i = 0; i < SEARCH_MAX_NODES; i++)
    visited[i] = 0;

  Stack s;
  stackInit(&s);
  stackPush(&s, startNode, 0);

  while (!stackIsEmpty(&s)) {
    int depth = 0;
    int u = stackPop(&s, &depth);
    if (!validNodeId(u) || visited[u])
      continue;

    visited[u] = 1;
    appendVisit(&order, u, depth, SEARCH_MAX_NODES);

    EdgeNode *edge = graph.getNeighbors(u);
    int neighbors[SEARCH_MAX_NODES];
    int nCount = 0;
    while (edge != nullptr && nCount < SEARCH_MAX_NODES) {
      neighbors[nCount++] = edge->to_node;
      edge = edge->next;
    }
    for (int i = nCount - 1; i >= 0; i--) {
      int v = neighbors[i];
      if (validNodeId(v) && !visited[v])
        stackPush(&s, v, depth + 1);
    }
  }

  return order;
}

IntList searchLocations(const Node *nodes, int nodeCount, const char *query) {
  IntList matches = makeIntList(nodeCount > 0 ? nodeCount : 1);
  if (!nodes || nodeCount <= 0 || !query || !matches.items)
    return matches;

  char qLower[64];
  toLowerCopy(query, qLower, (int)sizeof(qLower));
  if (qLower[0] == '\0')
    return matches;

  for (int i = 0; i < nodeCount; i++) {
    char nameLower[64];
    toLowerCopy(nodes[i].name, nameLower, (int)sizeof(nameLower));
    if (std::strstr(nameLower, qLower) != nullptr)
      appendInt(&matches, nodes[i].node_id, nodeCount);
  }

  return matches;
}

IntList findBfsPath(const Graph &graph, int startNode, int destNode) {
  IntList path;
  path.items = nullptr;
  path.count = 0;

  if (!validNodeId(startNode) || !validNodeId(destNode))
    return path;

  if (startNode == destNode) {
    path = makeIntList(1);
    appendInt(&path, startNode, 1);
    return path;
  }

  int visited[SEARCH_MAX_NODES];
  int parent[SEARCH_MAX_NODES];
  for (int i = 0; i < SEARCH_MAX_NODES; i++) {
    visited[i] = 0;
    parent[i] = -1;
  }

  Queue q;
  queueInit(&q);
  visited[startNode] = 1;
  enqueue(&q, startNode);

  int found = 0;
  while (!queueIsEmpty(&q)) {
    int u = dequeue(&q);
    if (u == destNode) {
      found = 1;
      break;
    }

    EdgeNode *edge = graph.getNeighbors(u);
    while (edge != nullptr) {
      int v = edge->to_node;
      if (validNodeId(v) && !visited[v]) {
        visited[v] = 1;
        parent[v] = u;
        enqueue(&q, v);
      }
      edge = edge->next;
    }
  }

  if (!found)
    return path;

  int reversePath[SEARCH_MAX_NODES];
  int len = 0;
  int curr = destNode;
  while (curr != -1 && len < SEARCH_MAX_NODES) {
    reversePath[len++] = curr;
    curr = parent[curr];
  }

  path = makeIntList(len);
  for (int i = len - 1; i >= 0; i--)
    appendInt(&path, reversePath[i], len);

  return path;
}

IntList findIntermediateDestinations(const Graph &graph, int startNode,
                                     int destNode) {
  IntList full = findBfsPath(graph, startNode, destNode);
  IntList mids;
  mids.items = nullptr;
  mids.count = 0;

  if (full.count <= 2) {
    freeIntList(&full);
    return mids;
  }

  int midCount = full.count - 2;
  mids = makeIntList(midCount);
  for (int i = 1; i < full.count - 1; i++)
    appendInt(&mids, full.items[i], midCount);

  freeIntList(&full);
  return mids;
}
