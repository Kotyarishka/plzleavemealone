// StateManager is a helper class to manage states of variables or components.
// It is used to store a state of a variable or component so that it can be
// retrieved later.

class StateManager {
    private state: Map<string, boolean | undefined> = new Map();

    constructor() {
        this.state = new Map();
    }

    /**
     * Sets the state of a variable or component.
     * @param key The key of the variable or component.
     * @param value The state of the variable or component.
     * @returns void
     * @memberof StateManager
     */
    public setState(key: string, value: boolean): void {
        this.state.set(key, value);
    }

    /**
     * Gets the state of a variable or component.
     * @param key The key of the variable or component.
     * @returns boolean | undefined
     * @memberof StateManager
     */
    public getState(key: string): boolean | undefined {
        return this.state.get(key);
    }

    public getLocalState(): Map<string, boolean | undefined> {
        return this.state;
    }

    /**
     * Checks if a variable or component has a state.
     * @param key The key of the variable or component.
     * @returns boolean
     * @memberof StateManager
     */
    public hasKey(key: string): boolean {
        return this.state.has(key);
    }

    /**
     * Deletes the state of a variable or component.
     * @param key The key of the variable or component.
     * @returns void
     * @memberof StateManager
     */
    public deleteKey(key: string): void {
        this.state.delete(key);
    }
}

export default StateManager;