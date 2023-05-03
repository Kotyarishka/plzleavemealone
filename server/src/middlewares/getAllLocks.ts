import { Request, Response, NextFunction } from 'express';
import LocksManager from '../classes/LocksManager';

import { RequestWithAllLocks } from '../interfaces/request';

export default function getAllLocks(req: Request, res: Response, next: NextFunction) {
    const locks = LocksManager.getAllLocks();

    let locksArray: Array<{ id: string, locked: boolean | undefined }> = [];

    locks.forEach((value, key) => {
        locksArray.push({ id: key, locked: value });
    });

    (req as RequestWithAllLocks).locks = locksArray;

    next();
}