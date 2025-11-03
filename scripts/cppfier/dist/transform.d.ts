import { Api, ApiTransform } from "./types";
export interface TransformConfig {
    sourceApi: Api;
    transform: ApiTransform;
}
/**
 * Transform the Api
 *
 * @param config the rules to apply into source
 */
export declare function transformApi(config: TransformConfig): {
    files: {};
    paramReplacements: any;
    delegatedReplacements: any;
};
