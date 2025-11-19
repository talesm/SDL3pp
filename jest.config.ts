import { createDefaultPreset } from "ts-jest";
import { type Config } from "jest";

const tsJestTransformCfg = createDefaultPreset().transform;

const config: Config = {
  testEnvironment: "node",
  transform: {
    ...tsJestTransformCfg,
  },
};
export default config;
