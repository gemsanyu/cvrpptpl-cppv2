#include "problem/node.h"

Node::Node(int index, float x, float y) : idx(index) {
  coord[0] = x;
  coord[1] = y;
}
