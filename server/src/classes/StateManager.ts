class StateManager {
    private state: Map<string, boolean | undefined> = new Map();

    constructor() {
        this.state = new Map();
    }

    public getState(lockId: string): boolean | null {
        return this.state.get(lockId) || null;
    }

    public setState(lockId: string, state: boolean | null): void {
        this.state.set(lockId, state || undefined);
    }
}

export default new StateManager();