# Guide on callbacks with a control flow

## Summary

**Do not:** Pass Eigen types by value (see [Eigen docs](https://eigen.tuxfamily.org/dox/group__TopicPassingByValue.html).

**Do:** Pass Eigen types using their expression base classes (see [Eigen docs](https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html).