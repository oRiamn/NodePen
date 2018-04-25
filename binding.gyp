{
  "targets": [
    {
      "target_name": "XinputAddon",
      "sources": [ "./XinputAddon/XinputAddon.cpp"],
      "include_dirs": [
      ],
      "libraries": [ 
        "-lX11",
        "-lXi",
        "-lm",
        "-lpthread"
       ]
    }
  ]
}