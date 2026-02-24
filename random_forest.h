/*
 * random_forest.h
 * EdgeCardia - Random Forest Arrhythmia Classifier
 * 
 * Model Performance: 90.26% accuracy
 * Memory: ~60KB Flash, ~2-3KB RAM
 */

#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include <stdint.h>
#include "normalization.h"


// Tree 0
static uint8_t tree_0(const float features[NUM_FEATURES]) {
    if (features[5] <= -0.180451f) {
        if (features[2] <= 0.204527f) {
            if (features[3] <= 0.111292f) {
                if (features[7] <= -1.135242f) {
                    if (features[1] <= -1.039061f) {
                        if (features[1] <= -1.144937f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.182060f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[3] <= -1.999005f) {
                        if (features[6] <= -2.260441f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.310564f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[1] <= -0.285471f) {
                    if (features[4] <= -0.070649f) {
                        if (features[7] <= -1.034161f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= -3.790966f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.094559f) {
                        if (features[1] <= -0.284789f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -0.703921f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[3] <= -0.592140f) {
                if (features[2] <= 0.331122f) {
                    if (features[5] <= -0.493111f) {
                        if (features[4] <= -0.018375f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.338987f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[5] <= -0.746357f) {
                        if (features[6] <= -2.333900f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 1.627976f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[7] <= 0.148643f) {
                    if (features[4] <= 0.169070f) {
                        if (features[3] <= -0.240424f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.237625f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[7] <= 1.576155f) {
                        if (features[3] <= -0.240424f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        }
    } else {
        if (features[3] <= -0.005946f) {
            if (features[7] <= -1.243456f) {
                if (features[6] <= 0.676094f) {
                    if (features[2] <= -0.496338f) {
                        if (features[4] <= 0.350457f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 0.295233f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[4] <= 1.222150f) {
                        if (features[2] <= -0.339538f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        return 0;
                    }
                }
            } else {
                if (features[6] <= -1.891888f) {
                    if (features[4] <= 0.516167f) {
                        if (features[4] <= -0.237630f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 0.389747f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= -0.370760f) {
                        if (features[7] <= -0.596461f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= 0.224228f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[5] <= 3.815013f) {
                if (features[1] <= 0.065824f) {
                    if (features[6] <= -0.041944f) {
                        if (features[1] <= -0.048999f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.446883f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[5] <= -0.044669f) {
                        if (features[3] <= 0.931964f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 1.645739f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[7] <= 2.781701f) {
                    if (features[1] <= 1.045197f) {
                        if (features[4] <= 3.140540f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        return 0;
                    }
                } else {
                    if (features[4] <= 3.324044f) {
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}

// Tree 1
static uint8_t tree_1(const float features[NUM_FEATURES]) {
    if (features[2] <= 0.098222f) {
        if (features[3] <= -0.005946f) {
            if (features[3] <= -2.116243f) {
                if (features[2] <= -0.907431f) {
                    if (features[2] <= -2.042706f) {
                        if (features[6] <= -2.249770f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= 0.309332f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[6] <= -2.717931f) {
                        return 1;
                    } else {
                        if (features[4] <= 0.917846f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[2] <= -0.449888f) {
                    if (features[4] <= -0.194110f) {
                        if (features[7] <= -0.630368f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -1.809428f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[3] <= -1.295572f) {
                        if (features[7] <= 0.643423f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 0.149524f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[4] <= -0.493267f) {
                if (features[6] <= 0.058000f) {
                    if (features[2] <= -0.529407f) {
                        if (features[7] <= -0.967778f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= 0.183499f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= -0.940259f) {
                        if (features[5] <= -0.452605f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.748341f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[3] <= 0.931964f) {
                    if (features[2] <= -0.218566f) {
                        if (features[3] <= 0.111292f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.217441f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= -0.063445f) {
                        if (features[7] <= -1.143345f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 0.557104f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        if (features[4] <= 0.139550f) {
            if (features[6] <= -0.281352f) {
                if (features[3] <= -0.826617f) {
                    if (features[5] <= -0.585992f) {
                        if (features[6] <= -1.376604f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -0.815726f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[5] <= -0.199927f) {
                        if (features[4] <= -0.148229f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= 0.419260f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[4] <= -0.474031f) {
                    if (features[1] <= 0.112215f) {
                        return 0;
                    } else {
                        if (features[6] <= 0.670264f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[3] <= 0.580247f) {
                        if (features[4] <= 0.018725f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[1] <= 0.349904f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[1] <= 0.579446f) {
                if (features[3] <= 0.814725f) {
                    if (features[3] <= -0.005946f) {
                        if (features[5] <= -0.651122f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= 0.580247f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[2] <= 0.289586f) {
                        if (features[6] <= 2.316649f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 1.015648f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[5] <= 0.208734f) {
                    if (features[7] <= -1.216028f) {
                        if (features[6] <= 0.837792f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -0.688083f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[7] <= 0.433047f) {
                        return 0;
                    } else {
                        if (features[2] <= 1.243563f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

// Tree 2
static uint8_t tree_2(const float features[NUM_FEATURES]) {
    if (features[4] <= 0.013980f) {
        if (features[2] <= -0.235992f) {
            if (features[2] <= -0.569248f) {
                if (features[7] <= -1.196059f) {
                    if (features[1] <= -1.233443f) {
                        if (features[7] <= -1.679974f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.271417f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[3] <= -0.826617f) {
                        if (features[2] <= -0.972984f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= 0.229893f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[7] <= 0.647437f) {
                    if (features[3] <= -0.357662f) {
                        if (features[2] <= -0.460588f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.568403f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[7] <= 1.064779f) {
                        if (features[4] <= -0.287985f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= -0.415517f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[2] <= 0.343813f) {
                if (features[7] <= 0.531341f) {
                    if (features[2] <= -0.050789f) {
                        if (features[5] <= -0.040905f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= 0.341012f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[3] <= 0.697486f) {
                        if (features[2] <= 0.099051f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[2] <= 0.223064f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[6] <= -0.248326f) {
                    if (features[3] <= -0.592140f) {
                        if (features[5] <= -0.629331f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.981021f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[3] <= 0.697486f) {
                        if (features[7] <= 1.919212f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= 0.858004f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        if (features[5] <= -0.578485f) {
            if (features[1] <= 0.889299f) {
                if (features[7] <= -1.162697f) {
                    if (features[3] <= 0.873344f) {
                        if (features[5] <= -1.043120f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= 1.283680f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= 0.503338f) {
                        if (features[4] <= 0.377383f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 2.675201f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[4] <= 0.453197f) {
                    if (features[4] <= 0.335469f) {
                        return 0;
                    } else {
                        return 0;
                    }
                } else {
                    if (features[5] <= -0.683545f) {
                        if (features[5] <= -1.454378f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        return 0;
                    }
                }
            }
        } else {
            if (features[3] <= -0.592140f) {
                if (features[2] <= -0.364575f) {
                    if (features[4] <= 1.455693f) {
                        if (features[5] <= 0.095701f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -1.056167f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.068385f) {
                        if (features[5] <= 3.489988f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= 1.393269f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[4] <= 0.864107f) {
                    if (features[3] <= -0.005946f) {
                        if (features[4] <= 0.135453f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 0.110756f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[5] <= 0.619321f) {
                        if (features[3] <= -0.005946f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 3.984853f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

// Tree 3
static uint8_t tree_3(const float features[NUM_FEATURES]) {
    if (features[2] <= 0.055986f) {
        if (features[6] <= -1.352412f) {
            if (features[7] <= -1.162869f) {
                if (features[4] <= 1.145552f) {
                    if (features[1] <= -1.924863f) {
                        if (features[5] <= 0.008155f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -1.295432f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    return 1;
                }
            } else {
                if (features[7] <= 1.209616f) {
                    if (features[4] <= 0.423082f) {
                        if (features[1] <= -0.329654f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -2.522035f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= -0.429151f) {
                        if (features[6] <= -2.274915f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.034286f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[4] <= -0.635718f) {
                if (features[1] <= -0.671105f) {
                    if (features[7] <= 0.611844f) {
                        if (features[7] <= -0.051258f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= 0.693691f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= -0.827245f) {
                        if (features[6] <= -0.780309f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= 1.283680f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[3] <= -0.005946f) {
                    if (features[2] <= -0.297457f) {
                        if (features[6] <= -0.067739f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -1.178334f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[3] <= 0.697486f) {
                        if (features[5] <= -0.528653f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.629546f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        if (features[7] <= 0.119879f) {
            if (features[6] <= 0.083536f) {
                if (features[7] <= -1.319678f) {
                    if (features[6] <= 0.070655f) {
                        if (features[5] <= -0.854242f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[3] <= -0.592140f) {
                        if (features[5] <= -0.586910f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.205098f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[5] <= -0.360134f) {
                    if (features[6] <= 0.452864f) {
                        if (features[3] <= 0.111292f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.664031f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= 0.266053f) {
                        if (features[6] <= 1.681059f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 1.481538f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[6] <= 2.127166f) {
                if (features[2] <= 0.349469f) {
                    if (features[2] <= 0.207821f) {
                        if (features[4] <= 1.224535f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= 0.742599f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= 0.725402f) {
                        if (features[6] <= -0.662625f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= -0.005946f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[6] <= 2.578249f) {
                    if (features[5] <= 0.107247f) {
                        return 1;
                    } else {
                        if (features[7] <= 0.675131f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[4] <= 3.102165f) {
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}

// Tree 4
static uint8_t tree_4(const float features[NUM_FEATURES]) {
    if (features[7] <= 0.237182f) {
        if (features[1] <= -0.214646f) {
            if (features[5] <= -0.596089f) {
                if (features[2] <= -1.204624f) {
                    if (features[2] <= -2.535364f) {
                        if (features[2] <= -2.957050f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.687026f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= -0.562979f) {
                        if (features[2] <= -0.733771f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= -0.215142f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[7] <= -1.880535f) {
                    if (features[3] <= 0.228531f) {
                        if (features[2] <= -2.307987f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.029138f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[6] <= -1.131557f) {
                        if (features[6] <= -2.413306f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.471572f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[3] <= -0.592140f) {
                if (features[5] <= -0.648284f) {
                    if (features[7] <= -1.182060f) {
                        if (features[1] <= 0.556488f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= -2.819676f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.236969f) {
                        if (features[5] <= 0.026163f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 0.500241f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[5] <= -0.221106f) {
                    if (features[2] <= 0.588203f) {
                        if (features[3] <= 0.580247f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.362759f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= -0.411305f) {
                        if (features[3] <= -0.005946f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 1.379208f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        if (features[4] <= 0.046368f) {
            if (features[6] <= -1.037693f) {
                if (features[3] <= 0.228531f) {
                    if (features[5] <= -0.198617f) {
                        if (features[1] <= 1.118195f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[1] <= -1.144289f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= -0.029230f) {
                        if (features[7] <= 1.260562f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= 0.598678f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[1] <= 0.042851f) {
                    if (features[5] <= 0.043623f) {
                        if (features[6] <= -0.081049f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.305650f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[5] <= -0.404883f) {
                        if (features[7] <= 0.737441f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= 0.683989f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[7] <= 0.993347f) {
                if (features[7] <= 0.310107f) {
                    if (features[3] <= -0.592140f) {
                        if (features[2] <= -1.300504f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[1] <= -0.243999f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= -2.090080f) {
                        if (features[3] <= -2.585198f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 0.475397f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[7] <= 2.239449f) {
                    if (features[2] <= -0.500865f) {
                        if (features[5] <= -1.224498f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 3.153080f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= 2.161588f) {
                        if (features[4] <= 2.145327f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= 5.971703f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}

// Tree 5
static uint8_t tree_5(const float features[NUM_FEATURES]) {
    if (features[1] <= 0.055931f) {
        if (features[2] <= -0.498578f) {
            if (features[5] <= -0.628865f) {
                if (features[4] <= -1.319735f) {
                    if (features[5] <= -0.632107f) {
                        if (features[4] <= -1.404186f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        return 0;
                    }
                } else {
                    if (features[1] <= -2.117322f) {
                        if (features[4] <= 0.370436f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= 1.807335f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[2] <= -0.940083f) {
                    if (features[7] <= -2.004311f) {
                        if (features[3] <= -0.943856f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -1.999005f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= -1.194392f) {
                        if (features[5] <= -0.485677f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -0.474901f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[6] <= -0.155549f) {
                if (features[5] <= 0.002982f) {
                    if (features[5] <= -0.196678f) {
                        if (features[5] <= -0.526207f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.229155f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[1] <= -0.289188f) {
                        if (features[1] <= -0.456332f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -2.116243f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[7] <= -0.131427f) {
                    if (features[5] <= -0.228476f) {
                        if (features[7] <= -1.769531f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 0.856597f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[5] <= 0.023925f) {
                        if (features[5] <= -0.440017f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= -1.178334f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        if (features[5] <= -0.231450f) {
            if (features[5] <= -0.777709f) {
                if (features[3] <= 0.580247f) {
                    if (features[2] <= 0.261429f) {
                        if (features[3] <= -2.174863f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= 1.310839f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[3] <= 1.107822f) {
                        return 0;
                    } else {
                        return 0;
                    }
                }
            } else {
                if (features[4] <= 0.159532f) {
                    if (features[7] <= -0.164443f) {
                        if (features[5] <= -0.369720f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= 0.187428f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[3] <= -0.005946f) {
                        if (features[6] <= -0.673098f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.600955f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[3] <= -0.005946f) {
                if (features[5] <= 4.871629f) {
                    if (features[7] <= 0.228800f) {
                        if (features[5] <= -0.215421f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= 1.124286f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= 0.337942f) {
                        return 1;
                    } else {
                        if (features[4] <= 1.017253f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[2] <= 0.218069f) {
                    if (features[2] <= 0.137181f) {
                        if (features[6] <= 2.306057f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= 1.283680f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[1] <= 1.208496f) {
                        if (features[4] <= -0.587401f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 0.680215f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

// Tree 6
static uint8_t tree_6(const float features[NUM_FEATURES]) {
    if (features[3] <= 0.111292f) {
        if (features[7] <= 0.468947f) {
            if (features[4] <= 0.072721f) {
                if (features[3] <= -0.709379f) {
                    if (features[1] <= -0.254342f) {
                        if (features[5] <= 0.086026f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -0.826617f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[1] <= -0.339745f) {
                        if (features[6] <= -0.571016f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.202499f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[1] <= -0.441408f) {
                    if (features[4] <= 1.371164f) {
                        if (features[5] <= 1.183037f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 0.756865f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.530963f) {
                        if (features[4] <= 0.415630f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.470250f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[4] <= 0.506780f) {
                if (features[5] <= -0.396345f) {
                    if (features[1] <= -0.025202f) {
                        if (features[7] <= 1.276809f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.748065f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= -0.053405f) {
                        if (features[1] <= -0.595419f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -0.474901f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[1] <= -1.201139f) {
                    if (features[6] <= -2.123119f) {
                        if (features[5] <= -2.174044f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= -2.033528f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[2] <= 1.171117f) {
                        if (features[4] <= 3.137719f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        }
    } else {
        if (features[3] <= 0.697486f) {
            if (features[2] <= 0.600988f) {
                if (features[5] <= -0.553980f) {
                    if (features[1] <= -0.645838f) {
                        if (features[3] <= 0.345770f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -1.014300f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[7] <= 0.429724f) {
                        if (features[1] <= 0.111560f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= -0.235683f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[2] <= 0.710933f) {
                    if (features[4] <= 0.185692f) {
                        if (features[4] <= -1.809903f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 0.241210f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= 0.287730f) {
                        if (features[4] <= 0.143387f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= -1.162140f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[7] <= 0.120790f) {
                if (features[2] <= -0.162384f) {
                    if (features[6] <= 1.234184f) {
                        if (features[2] <= -0.333367f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.519747f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.277006f) {
                        if (features[4] <= -0.299740f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= -0.425110f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[1] <= 0.052240f) {
                    if (features[4] <= -0.272250f) {
                        if (features[7] <= 0.648947f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= 1.932432f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= 2.328434f) {
                        if (features[3] <= 0.931964f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}

// Tree 7
static uint8_t tree_7(const float features[NUM_FEATURES]) {
    if (features[4] <= 0.020490f) {
        if (features[1] <= -0.049945f) {
            if (features[6] <= 0.731387f) {
                if (features[7] <= -1.091476f) {
                    if (features[1] <= -0.497251f) {
                        if (features[5] <= -0.631972f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -1.674959f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[4] <= -0.549738f) {
                        if (features[2] <= -0.582947f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[1] <= -0.329452f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[5] <= -0.067514f) {
                    if (features[3] <= 0.287151f) {
                        return 0;
                    } else {
                        if (features[2] <= -0.541106f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[5] <= 0.019790f) {
                        if (features[7] <= -0.112117f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= 0.608348f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[7] <= 0.183224f) {
                if (features[6] <= -0.344272f) {
                    if (features[5] <= -0.366623f) {
                        if (features[7] <= -0.554702f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[3] <= -0.709379f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= 0.225572f) {
                        if (features[3] <= -0.592140f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= 0.841601f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[4] <= -1.097885f) {
                    if (features[4] <= -1.851977f) {
                        if (features[2] <= 1.033950f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -1.405756f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= 0.348887f) {
                        if (features[3] <= -0.474901f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.945301f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        if (features[5] <= -0.595937f) {
            if (features[7] <= -1.127515f) {
                if (features[3] <= 1.166441f) {
                    if (features[3] <= -0.240424f) {
                        if (features[5] <= -0.656439f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= -0.982723f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[1] <= -0.808288f) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            } else {
                if (features[6] <= -2.089615f) {
                    if (features[1] <= -2.053835f) {
                        if (features[6] <= -2.173100f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -1.044264f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= 0.880743f) {
                        if (features[5] <= -0.939687f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -0.064566f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[4] <= 3.249202f) {
                if (features[2] <= 0.235961f) {
                    if (features[6] <= -1.302050f) {
                        if (features[4] <= 0.561280f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[2] <= -0.352511f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= 1.689536f) {
                        if (features[3] <= -0.005946f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 1.788657f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[2] <= -0.911291f) {
                    if (features[7] <= 3.001693f) {
                        return 1;
                    } else {
                        return 1;
                    }
                } else {
                    if (features[3] <= 1.049202f) {
                        if (features[3] <= 0.228531f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}

// Tree 8
static uint8_t tree_8(const float features[NUM_FEATURES]) {
    if (features[4] <= 0.023344f) {
        if (features[2] <= -0.111673f) {
            if (features[2] <= -0.493171f) {
                if (features[3] <= -0.005946f) {
                    if (features[2] <= -0.545241f) {
                        if (features[5] <= -0.361097f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= -0.551927f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= 0.304386f) {
                        if (features[7] <= -1.361138f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.523760f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[3] <= 0.111292f) {
                    if (features[1] <= -0.120012f) {
                        if (features[7] <= -0.448667f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[5] <= -0.259149f) {
                        if (features[7] <= 0.255375f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 0.043569f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[3] <= -0.592140f) {
                if (features[3] <= -0.826617f) {
                    if (features[7] <= -1.499275f) {
                        return 1;
                    } else {
                        if (features[2] <= 0.945454f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[4] <= -1.345368f) {
                        return 0;
                    } else {
                        if (features[3] <= -0.709379f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[5] <= -0.220241f) {
                    if (features[1] <= 0.348920f) {
                        if (features[2] <= -0.050546f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= -0.139588f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[7] <= 0.263715f) {
                        if (features[6] <= -0.796624f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= 0.315353f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        if (features[3] <= -0.592140f) {
            if (features[4] <= 1.453715f) {
                if (features[6] <= -1.302550f) {
                    if (features[2] <= -1.801306f) {
                        if (features[7] <= 1.253065f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[4] <= 0.398954f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[2] <= -0.209548f) {
                        if (features[1] <= -1.383650f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= -0.962665f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[7] <= 0.101773f) {
                    if (features[3] <= -0.709379f) {
                        if (features[7] <= 0.094263f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[1] <= -0.254404f) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            }
        } else {
            if (features[2] <= 0.290718f) {
                if (features[1] <= -0.520575f) {
                    if (features[3] <= 0.345770f) {
                        if (features[7] <= -0.992545f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 1.005509f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[5] <= 0.457271f) {
                        if (features[6] <= -0.323112f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[5] <= 3.772371f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[6] <= 0.211989f) {
                    if (features[4] <= 0.155628f) {
                        if (features[5] <= -0.565379f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.237625f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[6] <= 2.339328f) {
                        if (features[3] <= 0.931964f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[2] <= 0.712998f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

// Tree 9
static uint8_t tree_9(const float features[NUM_FEATURES]) {
    if (features[4] <= 0.016680f) {
        if (features[1] <= -0.266600f) {
            if (features[5] <= -0.562882f) {
                if (features[7] <= -0.722378f) {
                    if (features[5] <= -0.808741f) {
                        return 1;
                    } else {
                        if (features[7] <= -1.473594f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[1] <= -0.977644f) {
                        if (features[6] <= -2.450990f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[4] <= -1.444925f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[6] <= 0.698822f) {
                    if (features[2] <= -0.639003f) {
                        if (features[7] <= -1.359247f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= 0.601615f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= -0.329098f) {
                        if (features[5] <= -0.070638f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[2] <= -0.717608f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[5] <= -0.201121f) {
                if (features[6] <= -0.276103f) {
                    if (features[3] <= -0.592140f) {
                        if (features[4] <= -1.300468f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.949758f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[7] <= -0.393189f) {
                        if (features[1] <= 0.528621f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[6] <= -0.271400f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[7] <= -1.290816f) {
                    return 1;
                } else {
                    if (features[2] <= 0.115884f) {
                        if (features[5] <= -0.010634f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[3] <= -0.240424f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        if (features[2] <= 0.437039f) {
            if (features[6] <= -1.118239f) {
                if (features[3] <= -2.233482f) {
                    if (features[3] <= -2.585198f) {
                        if (features[7] <= 0.845504f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[1] <= -1.278737f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (features[4] <= 0.224964f) {
                        if (features[3] <= 0.404389f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[7] <= -1.580090f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[1] <= -0.341652f) {
                    if (features[7] <= -1.867562f) {
                        if (features[3] <= 0.228531f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= -0.961172f) {
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[1] <= 0.183299f) {
                        if (features[4] <= 0.487170f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= -0.770134f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        } else {
            if (features[3] <= -0.592140f) {
                if (features[5] <= -0.501872f) {
                    if (features[4] <= 0.366500f) {
                        return 0;
                    } else {
                        if (features[7] <= -1.163376f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= 1.107146f) {
                        if (features[1] <= 1.110633f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[5] <= 4.841953f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[1] <= 0.625833f) {
                    if (features[4] <= 0.809089f) {
                        if (features[5] <= -0.226745f) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[2] <= 0.622837f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[2] <= 1.238065f) {
                        if (features[3] <= 0.931964f) {
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        if (features[7] <= -0.489511f) {
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Random Forest prediction
 * Returns: 0 = Normal, 1 = Abnormal
 */
static inline uint8_t predict_random_forest(const float features[NUM_FEATURES]) {
    uint16_t votes = 0;
    
    votes += tree_0(features);
    votes += tree_1(features);
    votes += tree_2(features);
    votes += tree_3(features);
    votes += tree_4(features);
    votes += tree_5(features);
    votes += tree_6(features);
    votes += tree_7(features);
    votes += tree_8(features);
    votes += tree_9(features);

    return (votes > 5) ? 1 : 0;
}

#endif // RANDOM_FOREST_H
