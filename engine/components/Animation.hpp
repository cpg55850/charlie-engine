#pragma once

struct Animation {
  int mIndex;
  int mFrames;
  int mSpeed;

  Animation() {}
  Animation(int index, int frames, int speed) {
    mIndex = index;
    mFrames = frames;
    mSpeed = speed;
  }
};