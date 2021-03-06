# imalookalike
What celebrity is your lookalike?

### Deployments
We have [an instance of the application](http://imalookalike.me) deployed on DigitalOcean.

### Resources
For details look at [Model](https://github.com/slawiko/imalookalike/tree/master/src/app/server/net#imlookalike-model) and [Index](https://github.com/slawiko/imalookalike/tree/master/src/index#index) READMEs.
 * [Dataset](https://drive.google.com/drive/folders/0B7EVK8r0v71peklHb0pGdDl6R28) (must be unzipped)
 * [Dump](https://drive.google.com/file/d/1OD84hvLg5WMICFQhqX7K4E5S1rI6xJNN/view)
 * [Landmarks](https://github.com/AKSHAYUBHAT/TensorFace/blob/master/openface/models/dlib/shape_predictor_68_face_landmarks.dat)

### Build and run
To build and run the application locally:
 * Load all needed [resources](./README.md#Resources)
 * Specify paths to resources in environment variables
   * Linux\MacOS: `PATH_TO_DATASET=<path to folder with dataset> PATH_TO_DUMP=<path to dump file> PATH_TO_LANDMARKS=<path to landmarks file>`
   * Windows: `set PATH_TO_DATASET=<path to folder with dataset> & set PATH_TO_DUMP=<path to dump file> & set PATH_TO_LANDMARKS=<path to landmarks file>`
 * Install [Docker](https://docs.docker.com/install/) and use docker-compose:
   * Build with prepared images: `docker-compose up`
   * Build from source files: `docker-compose -f docker-compose-dev.yml up`

### Authors
 * [Darya Rovdo](https://github.com/DaryaRovdo)
   * Creating [network model](https://github.com/slawiko/imalookalike/tree/master/src/app/server/net) and Python service for using this model.
   * Dataset preprocessing and model training
   * Making [benchmark](https://github.com/slawiko/imalookalike/tree/master/benchmark/net) (comparison of the model with [Facenet](https://github.com/davidsandberg/facenet))

 * [Sviataslau Shchaurouski](https://github.com/slawiko)
   * Making [client](https://github.com/slawiko/imalookalike/tree/master/src/app/client)
   * Communication between client, model and index (including [REST server](https://github.com/slawiko/imalookalike/tree/master/src/app/server) for client).
   * Creating dockerfiles and docker-compose files. [Deployment](http://207.154.236.203) on DigitalOcean.

 * [Anton Grigoryev](https://github.com/batist73)
   * Creating [Index](https://github.com/slawiko/imalookalike/tree/master/src/index)
     * Implementing [HNSW](https://arxiv.org/abs/1603.09320) algorithm
     * Making REST server
